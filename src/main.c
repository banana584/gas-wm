#include <stdio.h>
#include <signal.h>
#include <sys/eventfd.h>
#include <wayland-server.h>
#include <wayland-client.h>
#include "../include/config/lua.h"
#include "../include/config/keys.h"
#include "../include/input/devices.h"
#include "../include/logger/logger.h"
#include "../include/data_structures/heap.h"

gas_event_handler* events;

void sighandle(int sig) {
    (void)sig;
    atomic_store(&events->stop, true);
}

static void listener_read(gas_event_handler* handler, gas_event_client* client) {
    (void)handler;
    gas_device_listener* listener = client->data;

    size_t count;
    read(client->self, &count, sizeof(size_t));

    gas_device_event incoming = event_queue_peek(&listener->handler->outgoing);

    struct udev_device* dev = ref_vector_get(&listener->handler->references, incoming.idx).device;
    const char* name = udev_device_get_sysattr_value(dev, "name");

    if (name) printf("%s\n", name);    

    gas_device_event outgoing = { .type = GAS_DEVICE_EVENT_RECV, .idx = incoming.idx };
    event_queue_push(&listener->handler->incoming, &outgoing);

    outgoing.type = GAS_DEVICE_EVENT_DEL;
    event_queue_push(&listener->handler->incoming, &outgoing);

    size_t val = 3;
    write(listener->handler->event_client->self, &val, sizeof(size_t));
}

static void listener_destroy(void* data) {
    free(data);
}

static bool less(void* left, void* right) {
    return *(int*)left < *(int*)right;
}
static bool greater(void* left, void* right) {
    return *(int*)left > *(int*)right;
}

DECLARE_HEAP(int_heap, int)
IMPL_HEAP(int_heap, int, false, less, greater)

int main() {
    int_heap heap = int_heap_create();

    int val = 1;
    int_heap_insert(&heap, &val);
    int_heap_insert(&heap, &val);
    val = 2;
    int_heap_insert(&heap, &val);
    
    printf("%d\n", int_heap_pop(&heap));
    printf("%d\n", int_heap_pop(&heap));
    printf("%d\n", int_heap_pop(&heap));
    
    int_heap_destroy(&heap);

    gas_logger_set_format("$DATETIME [$LEVEL] $MSG");
    GAS_LOGGER_LOG_NOARGS(GAS_LOG_LEVEL_INFO, "x");
    GAS_LOGGER_LOG(GAS_LOG_LEVEL_FATAL, "y %d", 1);

    lua_State* L = lua_init_state();

    lua_newtable(L);

    gas_keys_setup(L);

    lua_setglobal(L, "gas");

    lua_run_config(L);

    gas_keys_clean(L);

    lua_close_config(L);

    signal(SIGINT, sighandle);

    events = gas_events_create_handler();
    gas_device_handler* devices = gas_devices_create_handler(events);

    gas_device_listener* listener = (gas_device_listener*)malloc(sizeof(gas_device_listener));
    gas_event_client* listener_events = (gas_event_client*)malloc(sizeof(gas_event_client));

    listener->handler = devices;
    listener->client = listener_events;

    listener_events->self = eventfd(0, EFD_SEMAPHORE);
    listener_events->priority = 3;
    listener_events->data = listener;
    listener_events->read = listener_read;
    listener_events->destroy = listener_destroy;

    gas_devices_add_listener(devices, listener_events);
    gas_events_add_client(events, listener_events);

    gas_devices_enumerate(devices, "input");
    gas_devices_enumerate(devices, "drm");
    
    printf("\n");

    gas_events_run_handler(events);

    gas_events_destroy_handler(events);

    return 0;
}
