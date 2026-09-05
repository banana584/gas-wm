#include <stdio.h>
#include <signal.h>
#include <wayland-server.h>
#include <wayland-client.h>
#include "../include/config/lua.h"
#include "../include/config/keys.h"
#include "../include/input/devices.h"

gas_event_handler* events;

void sighandle(int sig) {
    (void)sig;
    atomic_store(&events->stop, true);
}

int main() {
    printf("Hello, World!\n");

    lua_State* L = lua_init_state();

    lua_newtable(L);

    gas_keys_setup(L);

    lua_setglobal(L, "gas");

    lua_run_config(L);

    gas_keys_clean(L);

    lua_close_config(L);

    signal(SIGINT, sighandle);

    events = gas_events_create_handler();
    size_t devices = gas_devices_create_handler(events);

    gas_events_run_handler(events);

    gas_events_del_client(events, devices);
    gas_events_destroy_handler(events);

    return 0;
}
