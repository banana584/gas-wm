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

/**
 * @struct gas_device_listener
 * @brief Represents a client listening for device changes.
*/
typedef struct gas_device_listener {
    /** A pointer to the device handler this is listening to. */
    gas_device_handler* handler;
    /** A pointer to the event listener this is from. */
    gas_event_client* client;
    
    /** The index in the listener vector. */
    size_t idx;
} gas_device_listener;

/**
 * @struct gas_device_reference
 * @brief Represents a device from udev that is still being used.
*/
typedef struct gas_device_reference {
    /** The udev device being referenced. */
    struct udev_device* device;
    /** The amount of listeners that are yet to recieve the event. */
    size_t recieved;
    /** The amount of listeners still using the device. */
    size_t count;
} gas_device_reference;

DECLARE_SLOT_MAP(ref_vector, gas_device_reference)

/**
 * @enum gas_device_event_type
 * @brief The type of event sent into a queue.
*/
typedef enum gas_device_event_type {
    GAS_DEVICE_EVENT_ADD, /** A new device has been added. */
    GAS_DEVICE_EVENT_RECV, /** The oldest device in the queue has been recieved by 1 client. */
    GAS_DEVICE_EVENT_DEL, /** A device can be deleted for 1 client. */
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

    /** Udev monitor event handler ptr. */
    gas_event_client* monitor_client;
    /** Incoming event queue handler ptr. */
    gas_event_client* event_client;

    /** Vector of all active device references. */
    ref_vector references;
    /** Vector of all listeners. */
    listener_vector listeners;
    
    /** A queue for all events from clients to handler. */
    event_queue incoming;
    /** A queue for all events from handler to clients. */
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
 * @warning Creating a handler automatically enumerates, so enumeration isn't required directly after creation.
 *
 * @param[in] handler The device handler to enumerate with.
 * @param[in] subsystem The udev subsystem to scan.
 */
void gas_devices_enumerate(gas_device_handler* handler, const char* subsystem);

/**
 * @brief Adds a listener to a device handler.
 *
 * @param[in] handler The device handler to add a listener to.
 * @param[in] client The event client to add to the device handler.
 *
 * @warning Event client must be added to event handler for events to be recieved.
*/
void gas_devices_add_listener(gas_device_handler* handler, gas_event_client* client);

/**
 * @brief Deletes a listener from a device handler.
 *
 * @param[in] handler The device handler to delete a listener from.
 * @param[in] client The event client to delete from the device handler.
 *
 * @warning Event client must be destroyed from event handler after this to be fully destroyed.
*/
void gas_devices_del_listener(gas_device_handler* handler, gas_event_client* client);

#endif