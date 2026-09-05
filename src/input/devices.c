#include "../../include/input/devices.h"
#include <libudev.h>

static void setup_monitor(gas_device_handler* handler) {
    handler->monitor = udev_monitor_new_from_netlink(handler->udev, "udev");

    udev_monitor_filter_add_match_subsystem_devtype(handler->monitor, "input", NULL);

    udev_monitor_enable_receiving(handler->monitor);
}

static void read_monitor(gas_event_handler* events, gas_event_client* client) {
    (void)events;
    gas_device_handler* handler = client->data;

    struct udev_device* device = udev_monitor_receive_device(handler->monitor);

    const char* action = udev_device_get_action(device);
    const char* devnode = udev_device_get_devnode(device);
    const char* name = udev_device_get_sysattr_value(device, "name");

    if (!devnode) {
        udev_device_unref(device);
        return;
    }

    printf("%s\n\tname: %s\n\tdevnode: %s\n\n", action, name ? (name) : "unknown", devnode);

    udev_device_unref(device);
}

size_t gas_devices_create_handler(gas_event_handler* events) {
    gas_device_handler* handler = (gas_device_handler*)malloc(sizeof(gas_device_handler));

    handler->udev = udev_new();

    gas_devices_enumerate(handler, "input");

    setup_monitor(handler);

    int monitor_fd = udev_monitor_get_fd(handler->monitor);

    gas_event_client* client = (gas_event_client*)malloc(sizeof(gas_event_client));
    client->self = monitor_fd;
    client->read = read_monitor;
    client->destroy = (gas_event_destroy)gas_devices_destroy_handler;
    client->data = handler;

    handler->client = gas_events_add_client(events, client);

    return handler->client;
}

void gas_devices_destroy_handler(gas_device_handler* handler) {
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
