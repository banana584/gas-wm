#include "../../include/events/handler.h"

gas_event_handler* gas_events_create_handler() {
    gas_event_handler* handler = (gas_event_handler*)malloc(sizeof(gas_event_handler));
    atomic_store(&handler->stop, false);
    handler->bitmap = UINT64_MAX;

    handler->epfd = epoll_create1(0);

    return handler;
}

void gas_events_destroy_handler(gas_event_handler* handler) {
    for (size_t i = 0; i < 64; i++) {
        if ((handler->bitmap >> i & 1) == 1) continue;

        gas_events_del_client(handler, i);
    }

    close(handler->epfd);

    free(handler);
}

size_t gas_events_add_client(gas_event_handler* handler, gas_event_client* client) {
    size_t pos = handler->bitmap ? __builtin_ctzll(handler->bitmap) : -1;
    if (pos == -1) {
        return -1;
    }

    handler->bitmap &= ~(1ULL << pos);

    handler->clients[pos] = client;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = client;
    epoll_ctl(handler->epfd, EPOLL_CTL_ADD, client->self, &ev);

    return pos;
}

void gas_events_del_client(gas_event_handler* handler, const size_t idx) {
    handler->bitmap |= (1ULL << idx);

    gas_event_client* client = handler->clients[idx];

    epoll_ctl(handler->epfd, EPOLL_CTL_DEL, client->self, NULL);

    client->destroy(client->data);
    close(client->self);
    free(client);
}

void gas_events_run_handler(gas_event_handler* handler) {
    while (true) {
        if (atomic_load(&handler->stop)) {
            break;
        }

        size_t nfds = epoll_wait(handler->epfd, handler->events, _GAS_HANDLER_MAX_EVENTS, -1);
        if (nfds == (size_t)-1) {
            continue;
        }

        for (size_t i = 0; i < nfds; i++) {
            struct epoll_event ev = handler->events[i];
            gas_event_client* client = ev.data.ptr;

            client->read(handler, client);
        }
    }
}
