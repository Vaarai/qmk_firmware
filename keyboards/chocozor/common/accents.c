/* Copyright 2024 Vaarai
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Accented letters, typed the way the host OS understands:
 * - Windows: Alt + numpad codes (SS_ACCENT_* in keymap.h).
 * - Linux: the Compose key, which the host maps to Menu for this keyboard
 *   (xkb option compose:menu), then two keys, e.g. Compose ' e -> é.
 */

#include "accents.h"
#include "keymap.h"
#include "os_detection.h"
#include "eeconfig.h"

/* Linux: Compose (Menu) + sequence from the default en_US.UTF-8 Compose table */
#define SS_COMPOSE(seq) SS_TAP(X_APP) seq

typedef struct {
    const char *lower;
    const char *upper;
} accent_strings_t;

static const accent_strings_t accents_windows[ACC_COUNT] = {
    [ACC_A_GRAVE] = {SS_ACCENT_MIN_A_GRAVE, SS_ACCENT_MAJ_A_GRAVE},
    [ACC_A_CIRCU] = {SS_ACCENT_MIN_A_CIRCU, SS_ACCENT_MAJ_A_CIRCU},
    [ACC_C_CEDIL] = {SS_ACCENT_MIN_C_CEDIL, SS_ACCENT_MAJ_C_CEDIL},
    [ACC_E_ACUTE] = {SS_ACCENT_MIN_E_ACUTE, SS_ACCENT_MAJ_E_ACUTE},
    [ACC_E_GRAVE] = {SS_ACCENT_MIN_E_GRAVE, SS_ACCENT_MAJ_E_GRAVE},
    [ACC_E_CIRCU] = {SS_ACCENT_MIN_E_CIRCU, SS_ACCENT_MAJ_E_CIRCU},
    [ACC_I_CIRCU] = {SS_ACCENT_MIN_I_CIRCU, SS_ACCENT_MAJ_I_CIRCU},
    [ACC_I_TREMA] = {SS_ACCENT_MIN_I_TREMA, SS_ACCENT_MAJ_I_TREMA},
    [ACC_O_CIRCU] = {SS_ACCENT_MIN_O_CIRCU, SS_ACCENT_MAJ_O_CIRCU},
    [ACC_O_TREMA] = {SS_ACCENT_MIN_O_TREMA, SS_ACCENT_MAJ_O_TREMA},
    [ACC_U_CIRCU] = {SS_ACCENT_MIN_U_CIRCU, SS_ACCENT_MAJ_U_CIRCU},
    [ACC_U_GRAVE] = {SS_ACCENT_MIN_U_GRAVE, SS_ACCENT_MAJ_U_GRAVE},
    [ACC_EURO]    = {SS_ACCENT_EURO, SS_ACCENT_EURO},
};

static const accent_strings_t accents_linux[ACC_COUNT] = {
    [ACC_A_GRAVE] = {SS_COMPOSE("`a"), SS_COMPOSE("`A")},
    [ACC_A_CIRCU] = {SS_COMPOSE("^a"), SS_COMPOSE("^A")},
    [ACC_C_CEDIL] = {SS_COMPOSE(",c"), SS_COMPOSE(",C")},
    [ACC_E_ACUTE] = {SS_COMPOSE("'e"), SS_COMPOSE("'E")},
    [ACC_E_GRAVE] = {SS_COMPOSE("`e"), SS_COMPOSE("`E")},
    [ACC_E_CIRCU] = {SS_COMPOSE("^e"), SS_COMPOSE("^E")},
    [ACC_I_CIRCU] = {SS_COMPOSE("^i"), SS_COMPOSE("^I")},
    [ACC_I_TREMA] = {SS_COMPOSE("\"i"), SS_COMPOSE("\"I")},
    [ACC_O_CIRCU] = {SS_COMPOSE("^o"), SS_COMPOSE("^O")},
    [ACC_O_TREMA] = {SS_COMPOSE("\"o"), SS_COMPOSE("\"O")},
    [ACC_U_CIRCU] = {SS_COMPOSE("^u"), SS_COMPOSE("^U")},
    [ACC_U_GRAVE] = {SS_COMPOSE("`u"), SS_COMPOSE("`U")},
    [ACC_EURO]    = {SS_COMPOSE("=e"), SS_COMPOSE("=e")},
};

/* The OS mode lives in the user EEPROM word, tagged so a blank EEPROM reads AUTO */
#define OS_MODE_TAG 0xA5000000u
#define OS_MODE_TAG_MASK 0xFF000000u

static os_mode_t os_mode = OS_MODE_AUTO;

void accents_init(void) {
    uint32_t raw = eeconfig_read_user();
    if ((raw & OS_MODE_TAG_MASK) == OS_MODE_TAG && (raw & 0xFF) < OS_MODE_COUNT) {
        os_mode = (os_mode_t)(raw & 0xFF);
    } else {
        os_mode = OS_MODE_AUTO;
    }
}

void os_mode_cycle(void) {
    os_mode = (os_mode_t)((os_mode + 1) % OS_MODE_COUNT);
    eeconfig_update_user(OS_MODE_TAG | (uint32_t)os_mode);
}

os_mode_t os_mode_get(void) {
    return os_mode;
}

bool os_is_linux(void) {
    switch (os_mode) {
        case OS_MODE_LINUX:
            return true;
        case OS_MODE_WINDOWS:
            return false;
        default:
            /* Unsure or macOS: keep the Windows codes, as before */
            return detected_host_os() == OS_LINUX;
    }
}

const char *os_mode_label(void) {
    if (os_mode == OS_MODE_AUTO) {
        return os_is_linux() ? "LNX" : "WIN";
    }
    return os_is_linux() ? "LNX*" : "WIN*";
}

void send_accent(accent_t accent) {
    if (accent >= ACC_COUNT) {
        return;
    }
    const accent_strings_t *table = os_is_linux() ? accents_linux : accents_windows;
    if (get_mods() & MOD_MASK_SHIFT) {
        del_mods(MOD_MASK_SHIFT);
        send_string(table[accent].upper);
    } else {
        send_string(table[accent].lower);
    }
}
