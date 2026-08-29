#ifndef _GAS_INCLUDE_EVENTS_HANDLER_H
#define _GAS_INCLUDE_EVENTS_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/epoll.h>

#ifndef _GAS_HANDLER_MAX_EVENTS
#define _GAS_HANDLER_MAX_EVENTS 1024
#endif

#ifndef _GAS_HANDLER_MAX_CLIENTS
#define _GAS_HANDLER_MAX_CLIENTS 64
#endif

typedef struct gas_event_client gas_event_client;
typedef struct gas_event_handler gas_event_handler;

typedef void (*gas_event_read)(gas_event_handler* handler, gas_event_client* client);
typedef void (*gas_event_destroy)(void* data);

/**
 * @struct event_client
 * @brief Describes a source listening for events.
 */
typedef struct gas_event_client {
    /** The listening fd. For example, the server socket in tcp. */
    int self;

    /** The function that will be called when an event is raised for this client. */
    gas_event_read read;

    /** The function that will be called when the client is removed. */
    gas_event_destroy destroy;

    /** User data field. */
    void* data;
} gas_event_client;

/**
 * @struct event_handler
 * @brief Describes an epoll-based event loop handler.
 *
 * Maintains clients and epoll state by itself, calls client functions when events are raised.
 */
typedef struct gas_event_handler {
    /** Array of all clients subscribed to events. Clients must be created by malloc and are owned by the handler. */
    gas_event_client* clients[_GAS_HANDLER_MAX_CLIENTS];
    /** Bitmap of which slots are used. Used to quickly find empty array slots. */
    uint64_t bitmap;

    /** Array of each event raised. Reset each time events are raised. */
    struct epoll_event events[_GAS_HANDLER_MAX_EVENTS];
    /** File descriptor of epoll instance. */
    int epfd;
} gas_event_handler;

/**
 * @brief Creates an event handler.
 *
 * @return The created event handler. Not running.
 *
 * @warning Must destroy the event_handler when it is finished.
 * @see gas_events_destroy_handler
 */
gas_event_handler* gas_events_create_handler();

/**
 * @brief Destroys an event handler.
 *
 * @param[in] handler The finished event handler.
 *
 * @warning Must only be called once on each event handler.
 */
void gas_events_destroy_handler(gas_event_handler* handler);

/**
 * @brief Adds a client to an event handler.
 *
 * @param[in] handler The event handler to add a client to. Must not be NULL.
 * @param[in] client The client to add. Must not be NULL and created by malloc. Ownership is taken.
 * @return The index of the client. -1 on failure.
 */
size_t gas_events_add_client(gas_event_handler* handler, gas_event_client* client);

/**
 * @brief Removes a client from an event handler.
 *
 * @param[in] handler The event handler to remove a client from. Must not be NULL.
 * @param[in] idx The index of the client to remove. Same as the index returned by gas_events_add_client.
 */
void gas_events_del_client(gas_event_handler* handler, const size_t idx);

/**
 * @brief Runs an event handler.
 *
 * @param[in] handler The event handler to run.
 *
 * @warning Runs an infinite loop.
 */
void gas_events_run_handler(gas_event_handler* handler);

#endif
