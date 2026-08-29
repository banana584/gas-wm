#include "../../include/config/lua.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

static const char* config_path() {
    static char path[1024];

    snprintf(path, sizeof(path), "%s/.config/gas/config.lua", getenv("HOME"));

    return path;
}

lua_State* lua_init_state() {
    lua_State* L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "Failed to create new lua state");
        return NULL;
    }

    luaL_openlibs(L);

    return L;
}

int lua_run_config(lua_State* L) {
    if (luaL_dofile(L, config_path()) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);

        return -1;
    }

    return 0;
}

void lua_close_config(lua_State* L) {
    lua_close(L);
}
