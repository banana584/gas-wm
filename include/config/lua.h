#ifndef _GAS_INCLUDE_CONFIG_LUA_H
#define _GAS_INCLUDE_CONFIG_LUA_H

#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

lua_State* lua_init_state();

int lua_run_config(lua_State* L);

void lua_close_config(lua_State* L);

#endif