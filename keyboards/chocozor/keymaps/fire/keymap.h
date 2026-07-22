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

/* Trackpad constants */
#define TRACKPAD_DEFAULT_DPI 600

/* Layers definitions */
enum layers { _ALPHA, _NAV, _NUM, _ADJUST, _G0, _G1 };

/* - Custom Keycodes (CK_xxx) */
#define CK_SSHT LSG(KC_S)
#define CK_SELL LSFT(LCTL(KC_LEFT))
#define CK_SELR LSFT(LCTL(KC_RIGHT))

typedef enum user_custom_keycodes_t {
    CK_RKJMP = SAFE_RANGE, /* Warframe rocket/bullet jump */
    CK_DPII,
    CK_DPID,
} user_custom_keycodes_t;

/* - Accent letter using Alt + Numpad numbers */
#define SS_ACCENT_MIN_A_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P1) SS_UP(X_LALT) /* â */
#define SS_ACCENT_MIN_A_GRAVE SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P3) SS_UP(X_LALT) /* à */
#define SS_ACCENT_MIN_C_CEDIL SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P5) SS_UP(X_LALT) /* ç */
#define SS_ACCENT_MIN_E_ACUTE SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P0) SS_UP(X_LALT) /* é */
#define SS_ACCENT_MIN_E_GRAVE SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P8) SS_UP(X_LALT) /* è */
#define SS_ACCENT_MIN_E_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P6) SS_UP(X_LALT) /* ê */
#define SS_ACCENT_MIN_I_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P4) SS_TAP(X_P0) SS_UP(X_LALT) /* î */
#define SS_ACCENT_MIN_I_TREMA SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P3) SS_TAP(X_P9) SS_UP(X_LALT) /* ï */
#define SS_ACCENT_MIN_O_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P4) SS_TAP(X_P7) SS_UP(X_LALT) /* ô */
#define SS_ACCENT_MIN_O_TREMA SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P4) SS_TAP(X_P8) SS_UP(X_LALT) /* ö */
#define SS_ACCENT_MIN_U_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P5) SS_TAP(X_P0) SS_UP(X_LALT) /* û */
#define SS_ACCENT_MIN_U_GRAVE SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P5) SS_TAP(X_P1) SS_UP(X_LALT) /* ù */

#define SS_ACCENT_MAJ_A_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P8) SS_TAP(X_P2) SS_UP(X_LALT) /* Â */
#define SS_ACCENT_MAJ_A_GRAVE SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P8) SS_TAP(X_P3) SS_UP(X_LALT) /* À */
#define SS_ACCENT_MAJ_C_CEDIL SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P2) SS_TAP(X_P8) SS_UP(X_LALT) /* Ç */
#define SS_ACCENT_MAJ_E_ACUTE SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P4) SS_TAP(X_P4) SS_UP(X_LALT) /* É */
#define SS_ACCENT_MAJ_E_GRAVE SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P1) SS_TAP(X_P2) SS_UP(X_LALT) /* È */
#define SS_ACCENT_MAJ_E_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P1) SS_TAP(X_P0) SS_UP(X_LALT) /* Ê */
#define SS_ACCENT_MAJ_I_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P1) SS_TAP(X_P5) SS_UP(X_LALT) /* Î */
#define SS_ACCENT_MAJ_I_TREMA SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P1) SS_TAP(X_P6) SS_UP(X_LALT) /* Ï */
#define SS_ACCENT_MAJ_O_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P2) SS_TAP(X_P6) SS_UP(X_LALT) /* Ô */
#define SS_ACCENT_MAJ_O_TREMA SS_DOWN(X_LALT) SS_TAP(X_P1) SS_TAP(X_P5) SS_TAP(X_P3) SS_UP(X_LALT) /* Ö */
#define SS_ACCENT_MAJ_U_CIRCU SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P3) SS_TAP(X_P4) SS_UP(X_LALT) /* Û */
#define SS_ACCENT_MAJ_U_GRAVE SS_DOWN(X_LALT) SS_TAP(X_P2) SS_TAP(X_P3) SS_TAP(X_P5) SS_UP(X_LALT) /* Ù */

#define SS_ACCENT_EURO SS_DOWN(X_LALT) SS_TAP(X_P0) SS_TAP(X_P1) SS_TAP(X_P2) SS_TAP(X_P8) SS_UP(X_LALT) /* € */
