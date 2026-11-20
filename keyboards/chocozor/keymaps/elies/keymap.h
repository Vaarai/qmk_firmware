/* Copyright 2024 Vaarai
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include QMK_KEYBOARD_H
#include "keymap_extras/keymap_dvorak.h"

/* Trackpad constants */
#define TRACKPAD_DEFAULT_DPI 600

/* Layers definitions */
enum layers { _ALPHA, _NAV, _NUM, _ADJUST, _G0, _G1 };
_Static_assert(_NAV == TRACKPAD_SCROLL_LAYER, "config.h: TRACKPAD_SCROLL_LAYER must be _NAV");

/* - Custom Keycodes (CK_xxx) */

/* Home row mods: Ctrl Alt Gui | Gui Alt Ctrl */
#define HRM_A LCTL_T(DV_A)
#define HRM_O LALT_T(DV_O)
#define HRM_E LGUI_T(DV_E)
#define HRM_T RGUI_T(DV_T)
#define HRM_N LALT_T(DV_N)
#define HRM_S RCTL_T(DV_S)
#define HRM_SCLN LCTL_T(DV_SCLN)
#define CK_SELL LSFT(LCTL(KC_LEFT))
#define CK_SELR LSFT(LCTL(KC_RIGHT))

typedef enum user_custom_keycodes_t {
    CK_DPII = SAFE_RANGE,
    CK_DPID,
    CK_DRILL, /* starts or stops the keyhud drill */
    CK_MAGIC, /* MAGIC pointing on/off: a touch on the pad starts at the gaze */
} user_custom_keycodes_t;

/* hud.c */
void hud_record(uint16_t keycode, keyrecord_t *record);
void hud_drill(void);
