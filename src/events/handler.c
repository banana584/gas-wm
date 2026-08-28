#include "../../include/events/handler.h"

event_handler* gas_events_create_handler() {
    event_handler* handler = (event_handler*)malloc(sizeof(event_handler));
    handler->bitmap = UINT64_MAX;

    handler.epfd = epoll_create1(0);

    return handler;
}

void gas_events_destroy_handler(event_handler* handler) {
    close(handler->epfd);

    free(handler);
}

size_t gas_events_add_client(event_handler* handler, const event_client* client) {
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

void gas_events_del_client(event_handler* handler, const size_t idx) {
    handler->bitmap |= (1ULL << idx);

    event_client* client = handler->clients[idx];

    epoll_ctl(handler->epfd, EPOLL_CTL_DEL, client->self, NULL);

    close(client->self);
    free(client);
}

void gas_events_run_handler(event_handler* handler) {
    while (true) {
        size_t nfds = epoll_wait(handler->epfd, handler->events, _GAS_HANDLER_MAX_EVENTS, -1);

        for (size_t i = 0; i < nfds; i++) {
            struct epoll_event ev = handler->events[i];
            event_client* client = ev.data.ptr;

            client->read(handler, client);
        }
    }
}
