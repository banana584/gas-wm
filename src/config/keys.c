#include "../../include/config/keys.h"
#include <lauxlib.h>
#include <lua.h>

#define MAX_KEYBINDS 1024
static keybind keybinds[MAX_KEYBINDS] = {0};
static size_t num_keybinds = 0;

key gas_keys_parse_key(const char* str) {
    for (size_t i = 0; i < (sizeof(keys) / sizeof(keys[0])); i++) {
        if (strcmp(str, keys[i].name) == 0) {
            return keys[i].key;
        }
    }

    return GAS_KEY_NONE;
}

bool gas_keys_bind(const keybind* bind) {
    if (num_keybinds >= MAX_KEYBINDS) {
        return false;
    }

    keybinds[num_keybinds++] = *bind;

    return true;
}

static int l_keys_bind(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    lua_getfield(L, 1, "key");

    const char* key_name = luaL_checkstring(L, -1);

    lua_pop(L, 1);

    lua_getfield(L, 1, "modifiers");

    luaL_checktype(L, -1, LUA_TTABLE);

    mod modifiers = 0;
    size_t count = lua_rawlen(L, -1);

    for (size_t i = 1; i <= count; i++) {
        lua_rawgeti(L, -1, i);

        const char* mod = luaL_checkstring(L, -1);
        for (size_t j = 0; j < (sizeof(mods) / sizeof(mods[0])); j++) {
            if (strcmp(mod, mods[j].name) == 0) {
                modifiers |= mods[j].mod;
            }
        }

        lua_pop(L, 1);
    }

    if (modifiers == 0) modifiers = GAS_MOD_NONE;

    lua_pop(L, 1);

    lua_pushvalue(L, 2);

    int callback = luaL_ref(L, LUA_REGISTRYINDEX);

    keybind bind = { .key = gas_keys_parse_key(key_name), .mods = modifiers, .ref = callback };

    gas_keys_bind(&bind);

    printf("Binded %d with mods %X to %d\n", bind.key, bind.mods, bind.ref);

    return 0;
}

void gas_keys_setup(lua_State* L) {
    lua_newtable(L); // gas.keys

    for (size_t i = 0; i < (sizeof(keys) / sizeof(keys[0])); i++) {
        lua_pushinteger(L, keys[i].key);
        lua_setfield(L, -2, keys[i].name);
    }

    lua_pushcfunction(L, l_keys_bind);
    lua_setfield(L, -2, "bind");

    lua_setfield(L, -2, "keys");
}

void gas_keys_clean(lua_State* L) {
    for (size_t i = 0; i < num_keybinds; i++) {
        luaL_unref(L, LUA_REGISTRYINDEX, keybinds[i].ref);
    }
}
