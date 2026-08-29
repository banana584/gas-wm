#ifndef _GAS_INCLUDE_INPUT_DEVICES_H
#define _GAS_INCLUDE_INPUT_DEVICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <libudev.h> // Use udev over sd-device for compatabilty on non-systemd devices.
#include "../events/handler.h"

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
    size_t client;
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
 * @return The id of the device handler in the event handler.
 */
size_t gas_devices_create_handler(gas_event_handler* events);

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
 */
void gas_devices_enumerate(gas_device_handler* handler);

#endif
