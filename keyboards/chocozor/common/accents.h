/* Copyright 2024 Vaarai
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include QMK_KEYBOARD_H

/* Accented characters the keymap can type */
typedef enum {
    ACC_A_GRAVE, /* à À */
    ACC_A_CIRCU, /* â Â */
    ACC_C_CEDIL, /* ç Ç */
    ACC_E_ACUTE, /* é É */
    ACC_E_GRAVE, /* è È */
    ACC_E_CIRCU, /* ê Ê */
    ACC_I_CIRCU, /* î Î */
    ACC_I_TREMA, /* ï Ï */
    ACC_O_CIRCU, /* ô Ô */
    ACC_O_TREMA, /* ö Ö */
    ACC_U_CIRCU, /* û Û */
    ACC_U_GRAVE, /* ù Ù */
    ACC_EURO,    /* €   */
    ACC_COUNT
} accent_t;

/* How accents are typed: AUTO follows OS detection, the others force it */
typedef enum {
    OS_MODE_AUTO,
    OS_MODE_LINUX,
    OS_MODE_WINDOWS,
    OS_MODE_COUNT
} os_mode_t;

/* Load the saved OS mode (call from keyboard_post_init_user) */
void accents_init(void);
/* Split: the USB half sends the mode and detected OS to the other half, so
 * os_mode_label() works on both (call from housekeeping_task_user; needs
 * RPC_ID_USER_OS_SYNC in SPLIT_TRANSACTION_IDS_USER) */
void accents_sync_task(void);
/* Cycle AUTO -> LINUX -> WINDOWS and save it */
void os_mode_cycle(void);
os_mode_t os_mode_get(void);
/* True when accents are sent the Linux way (forced, or AUTO and Linux detected) */
bool os_is_linux(void);
/* Short OLED label: "LNX", "WIN", "LNX*" / "WIN*" when forced */
const char *os_mode_label(void);

/* Type an accented character, upper case when Shift is held */
void send_accent(accent_t accent);
