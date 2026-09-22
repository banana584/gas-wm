#ifndef _GAS_INCLUDE_INPUT_DEVICES_H
#define _GAS_INCLUDE_INPUT_DEVICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/eventfd.h>
#include <libudev.h> // Use udev over sd-device for compatabilty on non-systemd devices.
#include "../events/handler.h"
#include "../data_structures/ring.h"
#include "../data_structures/vector.h"
#include "../data_structures/queue.h"
#include "../data_structures/slot_map.h"

typedef struct gas_device_handler gas_device_handler;

typedef struct gas_device_listener {
    gas_device_handler* handler;
    gas_event_client* client;
} gas_device_listener;

typedef struct gas_device_reference {
    struct udev_device* device;
    size_t recieved;
    size_t count;
} gas_device_reference;

DECLARE_SLOT_MAP(ref_vector, gas_device_reference)

typedef enum gas_device_event_type {
    GAS_DEVICE_EVENT_ADD,
    GAS_DEVICE_EVENT_RECV,
    GAS_DEVICE_EVENT_DEL,
} gas_device_event_type;

typedef struct gas_device_event {
    gas_device_event_type type;
    size_t idx;
} gas_device_event;

DECLARE_SLOT_MAP(listener_vector, gas_device_listener)
DECLARE_RING(event_queue, gas_device_event)

/**
 * @struct gas_device_handler
 * @brief Handles udev context and event managing for devices.
 */
typedef struct gas_device_handler {
    /** Global udev context. */
    struct udev* udev;
    /** Udev monitor to handle hotplugging. */
    struct udev_monitor* monitor;

    /** Event handler id. */
    gas_event_client* monitor_client;

    gas_event_client* event_client;

    ref_vector references;
    listener_vector listeners;
    event_queue incoming;
    event_queue outgoing;
} gas_device_handler;

/**
 * @brief Initializes a device handler.
 *
 * Creates a device handler, with existing devices enumerated and hotplugging setup for event listening.
 *
 * @warning Must be destroyed with gas_events_del_client
 * @warning Hotplugging is only handled in the event loop, so that must be started.
 * @see gas_events_del_client
 *
 * @param[in] events The event handler to attach to.
 * @return The created device handler.
 */
gas_device_handler* gas_devices_create_handler(gas_event_handler* events);

/**
 * @brief Destroys an event handler.
 *
 * Called automatically by gas_events_del_client, which can be given the return of gas_devices_create_handler.
 *
 * @warning Handler must not be used after calling this
 *
 * @param[in] The device handler to be destroyed
 */
void gas_devices_destroy_handler(gas_device_handler* handler);

/**
 * @brief Enumerates all plugged in devices. Normally shouldn't have to be called.
 *
 * @warning Most likely shouldn't be called by the user, as devices are automatically enumerated.
 *
 * @param[in] handler The device handler to enumerate with.
 * @param[in] subsystem The udev subsystem to scan.
 */
void gas_devices_enumerate(gas_device_handler* handler, const char* subsystem);

void gas_devices_add_listener(gas_device_handler* handler, gas_event_client* client);

#endif