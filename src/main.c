#include <lua.h>
#include <stdio.h>
#include <wayland-server.h>
#include <wayland-client.h>
#include "../include/config/lua.h"
#include "../include/config/keys.h"
#include "../include/input/keys.h"

int main() {
    printf("Hello, World!\n");

    lua_State* L = lua_init_state();

    lua_newtable(L);

    gas_keys_setup(L);

    lua_setglobal(L, "gas");

    lua_run_config(L);

    gas_keys_clean(L);

    lua_close_config(L);

    return 0;
}
