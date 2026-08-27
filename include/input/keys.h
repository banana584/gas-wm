#ifndef _GAS_INCLUDE_INPUT_KEYS_H
#define _GAS_INCLUDE_INPUT_KEYS_H

#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <xkbcommon/xkbcommon.h>

#define _GAS_KEY_LIST(X) \
    X(NONE) \
    X(A) \
    X(B) \
    X(C) \
    X(D) \
    X(E) \
    X(F) \
    X(G) \
    X(H) \
    X(I) \
    X(J) \
    X(K) \
    X(L) \
    X(M) \
    X(N) \
    X(O) \
    X(P) \
    X(Q) \
    X(R) \
    X(S) \
    X(T) \
    X(U) \
    X(V) \
    X(W) \
    X(X) \
    X(Y) \
    X(Z) \
    X(1) \
    X(2) \
    X(3) \
    X(4) \
    X(5) \
    X(6) \
    X(7) \
    X(8) \
    X(9) \
    X(0) \
    X(SPACE) \
    X(ENTER) \
    X(ESC) \
    X(LEFT) \
    X(UP) \
    X(RIGHT) \
    X(DOWN) \
    X(F1) \
    X(F2) \
    X(F3) \
    X(F4) \
    X(F5) \
    X(F6) \
    X(F7) \
    X(F8) \
    X(F9) \
    X(F10) \
    X(F11) \
    X(F12) \
    X(SHIFT) \
    X(CTRL) \
    X(ALT) \
    X(SUPER)

#define _GAS_MOD_LIST(X) \
    X(NONE, 0) \
    X(SHIFT, 1) \
    X(CTRL, 2) \
    X(ALT, 3) \
    X(SUPER, 4)

/**
 * @enum key
 * @brief Descripes a key on a keyboard. For example, A-Z, 0-9, etc
 */
typedef enum key {
    #define X(_name) GAS_KEY_##_name,
    _GAS_KEY_LIST(X)
    #undef X
    GAS_NUM_KEYS
} key;

/**
 * @enum mod
 * @brief Describes a modifier on a keyboard. For example, holding shift changes a to A.
 */
typedef enum mod {
    #define X(name, shift) GAS_MOD_##name = 1 << shift,
    _GAS_MOD_LIST(X)
    #undef X
    GAS_NUM_MODS = 5
} mod;

/**
 * @struct keyname
 * @brief Describes a connection between a string and a key/mod.
 */
typedef struct keyname {
    /** Name of key/mod to be connected. Must not be NULL. */
    const char* name;

    /** Key or mod to be connected. Either can be set and used. */
    union {
        key key;
        mod mod;
    };

    /** Describes if this is a key or mod. False for key and true for mod. */
    bool type;
} keyname;

/**
 * @struct keybind
 * @brief Describes a binding between a key+mods and a lua reference.
 */
typedef struct keybind {
    /** Key that must be pressed to activate the keybind. */
    key key;
    /** Modifiers that must be active to activate the keybind. Can be MOD_NONE. */
    mod mods;

    /** Lua reference that will be called when the keybind is activated. */
    int ref;
} keybind;

/**
 * @brief Global array of supported keys.
 *
 * Lists every key in the _GAS_KEY_LIST macro to its name.
 */
extern const keyname keys[GAS_NUM_KEYS];
/**
 * @brief Global array of supported mods.
 *
 * Lists every mod in the _GAS_MOD_LIST macro to its name.
 */
extern const keyname mods[GAS_NUM_MODS];

typedef struct keyboard_handler {
    struct udev* udev;
    struct udev_enumerate* enumerate;

    struct xkb_context* context;
    struct xkb_keymap* keymap;
    struct xkb_state* state;
} keyboard_handler;

keyboard_handler gas_keys_create();

void gas_keys_destroy(keyboard_handler* keys);

#endif
