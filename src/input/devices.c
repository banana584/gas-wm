#include "../../include/input/devices.h"
#include <libudev.h>
#include <sys/eventfd.h>

IMPL_SLOT_MAP(listener_vector, gas_device_listener)
IMPL_SLOT_MAP(ref_vector, gas_device_reference)
IMPL_RING(event_queue, gas_device_event, true)

static void setup_monitor(gas_device_handler* handler) {
    handler->monitor = udev_monitor_new_from_netlink(handler->udev, "udev");

    udev_monitor_filter_add_match_subsystem_devtype(handler->monitor, "input", NULL);
    udev_monitor_filter_add_match_subsystem_devtype(handler->monitor, "drm", NULL);

    udev_monitor_enable_receiving(handler->monitor);
}

static void handle_event(gas_event_handler* events, gas_event_client* client) {
    size_t count;
    read(client->self, &count, sizeof(size_t));

    (void)events;
    gas_device_handler* handler = client->data;

    gas_device_event event = event_queue_pop(&handler->incoming);

    gas_device_reference ref = ref_vector_get(&handler->references, event.idx);

    if (event.type == GAS_DEVICE_EVENT_RECV) {
        ref.recieved--;

        if (ref.recieved == 0) {
            event_queue_pop(&handler->outgoing);
        }
    } else if (event.type == GAS_DEVICE_EVENT_DEL) {
        ref.count--;

        if (ref.count == 0) {
            udev_device_unref(ref.device);
            ref.device = NULL;
            ref_vector_pop(&handler->references, event.idx);
        }
    }
    
    ref_vector_set(&handler->references, event.idx, &ref);
}

static void handle_device(gas_device_handler* handler, struct udev_device* device) {
    gas_device_reference ref = { .device = device };

    size_t idx = ref_vector_push(&handler->references, &ref);

    gas_device_event event = { .type = GAS_DEVICE_EVENT_ADD, .idx = idx };

    event_queue_push(&handler->outgoing, &event);

    size_t val = 1;

    for (size_t i = 0; i < handler->listeners.cap; i++) {
        gas_device_listener listener = listener_vector_get(&handler->listeners, i);

        if (!listener.client) continue;

        write(listener.client->self, &val, sizeof(size_t));
    }

    ref.count = handler->listeners.count;
    ref.recieved = ref.count;

    ref_vector_set(&handler->references, idx, &ref);
}

static void read_monitor(gas_event_handler* events, gas_event_client* client) {
    (void)events;
    gas_device_handler* handler = client->data;

    handle_device(handler, udev_monitor_receive_device(handler->monitor));
}

gas_device_handler* gas_devices_create_handler(gas_event_handler* events) {
    gas_device_handler* handler = (gas_device_handler*)malloc(sizeof(gas_device_handler));
    handler->references = ref_vector_create();
    handler->listeners = listener_vector_create();
    handler->incoming = event_queue_create(1024);
    handler->outgoing = event_queue_create(1024);

    handler->udev = udev_new();

    gas_devices_enumerate(handler, "input");
    gas_devices_enumerate(handler, "drm");

    setup_monitor(handler);

    int monitor_fd = udev_monitor_get_fd(handler->monitor);

    int event_fd = eventfd(0, EFD_SEMAPHORE);

    handler->monitor_client = (gas_event_client*)malloc(sizeof(gas_event_client));
    handler->monitor_client->self = monitor_fd;
    handler->monitor_client->read = read_monitor;
    handler->monitor_client->destroy = (gas_event_destroy)gas_devices_destroy_handler;
    handler->monitor_client->data = handler;

    gas_events_add_client(events, handler->monitor_client);

    handler->event_client = (gas_event_client*)malloc(sizeof(gas_event_client));
    handler->event_client->self = event_fd;
    handler->event_client->read = handle_event;
    handler->event_client->destroy = NULL;
    handler->event_client->data = handler;

    gas_events_add_client(events, handler->event_client);

    return handler;
}

void gas_devices_destroy_handler(gas_device_handler* handler) {
    for (size_t i = 0; i < handler->references.count; i++) {
        gas_device_reference ref = ref_vector_get(&handler->references, i);
        if (ref.device) {
            udev_device_unref(ref.device);
        }
    }

    ref_vector_destroy(&handler->references);
    listener_vector_destroy(&handler->listeners);
    event_queue_destroy(&handler->incoming);
    event_queue_destroy(&handler->outgoing);
    udev_monitor_unref(handler->monitor);
    udev_unref(handler->udev);
    free(handler);
}

void gas_devices_enumerate(gas_device_handler* handler, const char* subsystem) {
    struct udev_enumerate* enumerate = udev_enumerate_new(handler->udev);

    udev_enumerate_add_match_subsystem(enumerate, subsystem);

    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* entry;
    udev_list_entry_foreach(entry, udev_enumerate_get_list_entry(enumerate)) {
        const char* syspath = udev_list_entry_get_name(entry);

        struct udev_device* device = udev_device_new_from_syspath(handler->udev, syspath);

        printf("device: %s\n", syspath);

        udev_device_unref(device);
    }

    udev_enumerate_unref(enumerate);
}

void gas_devices_add_listener(gas_device_handler* handler, gas_event_client* client) {
    listener_vector_push(&handler->listeners, client->data);
}