#ifndef _GAS_INCLUDE_CONFIG_KEYS_H
#define _GAS_INCLUDE_CONFIG_KEYS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../input/keys.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/**
 * @brief Parses a key string like "A" or "ENTER" into the key enum.
 *
 * @param[in] str The string to parse into a key. Must not be NULL.
 *
 * @return The parsed key or GAS_KEY_NONE if no matching key was found.
 */
key gas_keys_parse_key(const char* str);

/**
 * @brief Adds a keybind into the keybind array.
 *
 * @param[in] bind The keybind requested to be added. Must not be NULL.
 *
 * @return True if the keybind was succesfully added.
 * @return False if the keybind was not added. Only happens due to the max keybind limit being exceeded.
 */
bool gas_keys_bind(const keybind* bind);

/**
 * @brief Sets up the lua config of gas keys
 *
 * Creates the table "keys" within the current table, and initialises all variables and functions related to key config.
 *
 * @param[in,out] L The current lua state. Must not be NULL and is modified.
 *
 * @warning gas_keys_clean must be called when gas is finished.
 * @see gas_keys_clean
 */
void gas_keys_setup(lua_State* L);

/**
 * @brief Cleans up the lua config of gas keys.
 *
 * @param[in,out] L The current lua state. Must not be NULL and is modified.
 *
 * @warning This must not be called before gas_keys_setup.
 * @warning gas.keys and any keybindings must not be used after this is called.
 * @see gas_keys_setup
 */
void gas_keys_clean(lua_State* L);

#endif
