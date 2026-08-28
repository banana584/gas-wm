#ifndef _GAS_INCLUDE_EVENTS_HANDLER_H
#define _GAS_INCLUDE_EVENTS_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/epoll.h>

#ifndef _GAS_HANDLER_MAX_EVENTS
#define _GAS_HANDLER_MAX_EVENTS 1024
#endif

#ifndef _GAS_HANDLER_MAX_CLIENTS
#define _GAS_HANDLER_MAX_CLIENTS 64
#endif

typedef struct event_client event_client;
typedef struct event_handler event_handler;

typedef void (*gas_event_read)(event_handler* handler, event_client* client);

/**
 * @struct event_client
 * @brief Describes a source listening for events.
 */
typedef struct event_client {
    /** The listening fd. For example, the server socket in tcp. */
    int self;

    /** The function that will be called when an event is raised for this client. */
    gas_event_read read;
} event_client;

/**
 * @struct event_handler
 * @brief Describes an epoll-based event loop handler.
 *
 * Maintains clients and epoll state by itself, calls client functions when events are raised.
 */
typedef struct event_handler {
    /** Array of all clients subscribed to events. Clients must be created by malloc and are owned by the handler. */
    event_client* clients[_GAS_HANDLER_MAX_CLIENTS];
    /** Bitmap of which slots are used. Used to quickly find empty array slots. */
    uint64_t bitmap;

    /** Array of each event raised. Reset each time events are raised. */
    struct epoll_event events[_GAS_HANDLER_MAX_EVENTS];
    /** File descriptor of epoll instance. */
    int epfd;
} event_handler;

/**
 * @brief Creates an event handler.
 *
 * @return The created event handler. Not running.
 *
 * @warning Must destroy the event_handler when it is finished.
 * @see gas_events_destroy_handler
 */
event_handler* gas_events_create_handler();

/**
 * @brief Destroys an event handler.
 *
 * @param[in] handler The finished event handler.
 *
 * @warning Must only be called once on each event handler.
 */
void gas_events_destroy_handler(event_handler* handler);

/**
 * @brief Adds a client to an event handler.
 *
 * @param[in] handler The event handler to add a client to. Must not be NULL.
 * @param[in] client The client to add. Must not be NULL and created by malloc. Ownership is taken.
 * @return The index of the client. -1 on failure.
 */
size_t gas_events_add_client(event_handler* handler, const event_client* client);

/**
 * @brief Removes a client from an event handler.
 *
 * @param[in] handler The event handler to remove a client from. Must not be NULL.
 * @param[in] idx The index of the client to remove. Same as the index returned by gas_events_add_client.
 */
void gas_events_del_client(event_handler* handler, const size_t idx);

/**
 * @brief Runs an event handler.
 *
 * @param[in] handler The event handler to run.
 *
 * @warning Runs an infinite loop.
 */
void gas_events_run_handler(event_handler* handler);

#endif
