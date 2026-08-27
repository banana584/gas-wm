#include "../../include/input/keys.h"
#include <libudev.h>
#include <xkbcommon/xkbcommon.h>

const keyname keys[GAS_NUM_KEYS] = {
    #define X(_name) { #_name, .key = GAS_KEY_##_name, .type = 0 },
    _GAS_KEY_LIST(X)
    #undef X
};
const keyname mods[GAS_NUM_MODS] = {
    #define X(_name, _shift) { .name = #_name, .mod = GAS_MOD_##_name, .type = 1 },
    _GAS_MOD_LIST(X)
    #undef X
};

keyboard_handler gas_keys_create() {
    keyboard_handler keys;

    keys.udev = udev_new();
    if (!keys.udev) {
        perror("udev_new");
        return keys;
    }

    keys.enumerate = udev_enumerate_new(keys.udev);
    if (!keys.enumerate) {
        perror("udev_enumerate_new");
        udev_unref(keys.udev);
        return keys;
    }

    udev_enumerate_add_match_subsystem(keys.enumerate, "input");
    if (udev_enumerate_scan_devices(keys.enumerate) < 0) {
        perror("udev_enumerate_scan_devices");
        udev_enumerate_unref(keys.enumerate);
        udev_unref(keys.udev);
        return keys;
    }

    keys.context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

    struct xkb_rule_names names = {
        .rules = NULL,
        .model = NULL,
        .layout = "gb",
        .variant = NULL,
        .options = NULL
    };
    keys.keymap = xkb_keymap_new_from_names(keys.context, &names, XKB_KEYMAP_COMPILE_NO_FLAGS);

    keys.state = xkb_state_new(keys.keymap);

    return keys;
}

void gas_keys_destroy(keyboard_handler* keys) {
    xkb_state_unref(keys->state);
    xkb_keymap_unref(keys->keymap);
    xkb_context_unref(keys->context);
}
