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

#include QMK_KEYBOARD_H

#include "./keymap.h"
#include "./tap_dances.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_ALPHA] = LAYOUT_split_3x6_4( /* Dvorak, keycodes for an OS set to us(dvorak) */
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_TAB, DV_QUOT, DV_COMM,  DV_DOT,    DV_P,    DV_Y,                         DV_F,    DV_G,    DV_C,    DV_R,    DV_L, DV_SLSH,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,   HRM_A,   HRM_O,   HRM_E,    DV_U,    DV_I,                         DV_D,    DV_H,   HRM_T,   HRM_N,   HRM_S, DV_MINS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, DV_SCLN,    DV_Q,    DV_J,    DV_K,    DV_X,                         DV_B,    DV_M,    DV_W,    DV_V,    DV_Z, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
               KC_LGUI, KC_SPC, MO(_NAV), TD(TD_SFT_CAPSW),   TD(TD_SFT_CAPSW), MO(_NUM), KC_BSPC,  KC_ENT
                             //`-----------------------------------'  `-----------------------------------'
  ),

    [_NAV] = LAYOUT_split_3x6_4(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       QK_RBT, C(DV_S), KC_PSCR, C(DV_F), C(DV_A), C(DV_Y),                      MS_BTN1, MS_BTN2, MS_BTN3, XXXXXXX, XXXXXXX,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                      MS_WHLU, KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
   A(KC_LEFT), C(DV_X), G(DV_V), C(DV_V), C(DV_C), C(DV_Z),                      MS_WHLD, KC_HOME, CK_SELL, CK_SELR,  KC_END, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                 _______, _______, _______, _______,    _______,MO(_ADJUST), _______, _______
                             //`-----------------------------------'  `-----------------------------------'
  ),

    [_NUM] = LAYOUT_split_3x6_4(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
  //|--------+-- / ---+-- { ---+-- ( ---+-- [ ---+-- - ---|                    |--- _ --+--- ] --+-- ) ---+-- } ---+-- \ ---+--------|
      _______, DV_SLSH, DV_LCBR, DV_LPRN, DV_LBRC, DV_MINS,                      DV_UNDS, DV_RBRC, DV_RPRN, DV_RCBR, DV_BSLS, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX,  DV_GRV, DV_QUES,  DV_EQL, XXXXXXX,                      XXXXXXX, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                               _______, _______,MO(_ADJUST), _______,    _______, _______, _______, _______
                             //`-----------------------------------'  `-----------------------------------'
  ),

    [_ADJUST] = LAYOUT_split_3x6_4(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, UG_TOGG, UG_HUEU, UG_VALU, CK_DPII, XXXXXXX,                        KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|--------+--------+--------+--------+--------|--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, UG_NEXT, UG_HUED, UG_VALD, CK_DPID, XXXXXXX,                      TO(_G0), TO(_G1), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                 _______, _______, _______, _______,    _______, _______, _______, _______
                             //`-----------------------------------'  `-----------------------------------'
  ),

  /* Generic game */
    [_G0] = LAYOUT_split_3x6_4(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,  KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,                         KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LALT, KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,                         KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,                         KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                 _______,  KC_SPC,  KC_SPC, MS_BTN3,    XXXXXXX, TG(_G0), XXXXXXX, _______
                             //`-----------------------------------'  `-----------------------------------'
  ),

  /* Warframe */
    [_G1] = LAYOUT_split_3x6_4(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_TAB,  KC_ESC,    KC_Q,    KC_W,    KC_1,    KC_2,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         KC_F, KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_3,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LCTL,    KC_M,    KC_Y,    KC_X,    KC_4,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                  _______, KC_SPC,CK_RKJMP,    KC_5,    XXXXXXX, TG(_G1), XXXXXXX, _______
                             //`-----------------------------------'  `-----------------------------------'
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    
    if(IS_USER_KEYCODE(keycode))
    {
        switch (keycode) {
            case CK_RKJMP: /* Warframe rocket jump */
                if (record->event.pressed) {
                    SEND_STRING(SS_DOWN(X_C));
                } else {
                    SEND_STRING(SS_DOWN(X_SPC) SS_DELAY(50) SS_UP(X_C) SS_DELAY(50) SS_UP(X_SPC));
                }
                return false;
            case CK_DPII: /* Increase trackpad DPI */
                if (record->event.pressed) {
                    pointing_device_set_cpi(pointing_device_get_cpi()+100);
                }
                return false;
            case CK_DPID: /* Decrease trackpad DPI */
                if (record->event.pressed) {
                    pointing_device_set_cpi(pointing_device_get_cpi()-100);
                }
                return false;
        }
    }
    return true;
}

bool shutdown_user(bool jump_to_bootloader) {
    oled_clear();
    oled_set_cursor(0, 2);
    if (jump_to_bootloader) {
        oled_write_P(PSTR("FLASH"), false);
    } else {
        oled_write_P(PSTR("RESET"), false);
    }
    oled_render_dirty(true);
    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    char dpi_string[6] = {0};

    /* Print cornia logo */
    chocozor_render_logo();
    
    /* Clear DPI display */
    oled_set_cursor(0, 13);
    oled_write_ln(dpi_string, 0);

    /* Print layer status */
    oled_set_cursor(0, 7);
    switch (get_highest_layer(layer_state)) {
        case _ALPHA:
            oled_write_ln("ABCDE", 0);
            break;
        case _NAV:
            oled_write_ln("<--->", 0);
            break;
        case _NUM:
            oled_write_ln("!0@*8", 0);
            break;
        case _ADJUST:
            oled_write_ln("00000", 0);
            /* Print DPI value */
            snprintf(dpi_string, 6, "%5d", pointing_device_get_cpi());
            oled_set_cursor(0, 13);
            oled_write_ln(dpi_string, 0);
            break;
        case _G0:
            oled_write_ln("GAME0", 0);
            break;
        case _G1:
            oled_write_ln("GAME1", 0);
            break;
    }

    return false;
}

void pointing_device_init_user(void) {
    pointing_device_set_cpi(TRACKPAD_DEFAULT_DPI);
}

/* Knobs: index 0 is the left half's. */
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_ALPHA]  = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NAV]    = { ENCODER_CCW_CW(_______, _______),      ENCODER_CCW_CW(G(KC_LEFT), G(KC_RIGHT)) },
    [_NUM]    = { ENCODER_CCW_CW(C(DV_MINS), C(DV_EQL)), ENCODER_CCW_CW(_______, _______) },
    [_ADJUST] = { ENCODER_CCW_CW(_______, _______),      ENCODER_CCW_CW(_______, _______) },
    [_G0]     = { ENCODER_CCW_CW(_______, _______),      ENCODER_CCW_CW(_______, _______) },
    [_G1]     = { ENCODER_CCW_CW(_______, _______),      ENCODER_CCW_CW(_______, _______) },
};
