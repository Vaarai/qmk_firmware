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

#include "../../common/keymap.h"
#include "../../common/tap_dances.h"
#include "../../common/accents.h"
#include "../../common/pcstatus.h"
#include "../../common/trackpad.h"

#include "raw_hid.h"
#include "solenoid.h"
#include "transactions.h"

/* Keymap-only keycodes, after the common ones (common/keymap.h) */
enum {
    CK_MAGIC = CK_OSMD + 1, /* MAGIC pointing on/off */
};

#ifdef RGB_MATRIX_ENABLE
uint16_t check_rgblight_timer = 0;
uint8_t previous_rgb_matrix_mode = RGB_MATRIX_NONE;
hsv_t previous_rgb_matrix_hsv = (hsv_t){HSV_OFF};
#endif // RGB_MATRIX_ENABLE3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_ALPHA] = LAYOUT_2_9_regular( /* Fire (Oxey) : https://bit.ly/layout-doc-v2 */
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,    KC_P,    KC_L,    KC_D,    KC_G,    KC_V,                         KC_Q,    KC_F,    KC_O,    KC_U, KC_COMM, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_N,    KC_R,    KC_T,    KC_S,    KC_Y,                         KC_J,    KC_H,    KC_A,    KC_E,    KC_I, KC_LALT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_X,    KC_K,    KC_C,    KC_W,    KC_Z,                         KC_B,    KC_M, KC_QUOT, KC_SCLN,  KC_DOT, KC_RCTL,
  //|--------+--------+--------+--------+--------+--------'                    `--------+--------+--------+--------+--------+--------|
                        G(KC_Z), KC_LGUI, G(KC_G),                                        MS_BTN1, MS_BTN3, MS_BTN2,
  //                  `--------+--------+--------+-----------------`  '-----------------+--------+--------+--------'
                        TD(TD_SFT_CAPSW), TD(TD_NAV_ACCENT), KC_SPC,   KC_ENT, TD(TD_NUM_ACCENT), TD(TD_SFT_CAPSW)
  //                  `-----------------+------------------+-------'  `------+------------------+------------------'
  ),

    [_NAV] = LAYOUT_2_9_regular(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       QK_RBT, G(KC_L), XXXXXXX, XXXXXXX, C(KC_A), XXXXXXX,                      LSG(KC_COMM),G(KC_LEFT),G(KC_DOWN),G(KC_UP),G(KC_RIGHT), KC_DEL,
  //|--------+--------+ GUI V  +--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, C(KC_X), G(KC_V), C(KC_V), C(KC_C), C(KC_Z),                      MS_WHLU, KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, _______,
  //|ALT LEFT+--------+ ALT SFT+scrnshot+--------+--------|                    |--------+--------+--------+--------+--------+--------|
   A(KC_LEFT),C(KC_S),S(KC_LALT), CK_SSHT,C(KC_F), C(KC_Y),                      MS_WHLD, KC_HOME, CK_CTLL, CK_CTLR,  KC_END, _______,
  //|--------+--------+--------+--------+--------+--------'                    `--------+--------+--------+--------+--------+--------|
                        LAG(KC_Z), _______, _______,                                      _______, _______, _______,
  //                  `--------+--------+--------+-----------------`  '-----------------+--------+--------+--------'
                                          _______, _______, _______,  _______,MO(_ADJUST),_______
  //                                    `--------------------------'  `--------------------------'
  ),

    [_NUM] = LAYOUT_2_9_regular(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
  //|--------+-- / ---+-- { ---+-- ( ---+-- [ ---+-- - ---|                    |--- _ --+--- ] --+-- ) ---+-- } ---+-- \ ---+--------|
      _______, KC_SLSH, KC_LCBR, KC_LPRN, KC_LBRC, KC_MINS,                      KC_UNDS, KC_RBRC, KC_RPRN, KC_RCBR, KC_BSLS, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX,  KC_GRV, KC_QUES,  KC_EQL, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------'                    `--------+--------+--------+--------+--------+--------|
                        _______, _______, _______,                                        _______, _______, _______,
  //                  `--------+--------+--------+-----------------`  '-----------------+--------+--------+--------'
                                          _______,MO(_ADJUST),_______,    _______, _______, _______
  //                                    `--------------------------'  `--------------------------'
  ),

    [_ADJUST] = LAYOUT_2_9_regular(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, CK_OSMD, AC_TOGG,                        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, UG_TOGG, UG_HUEU, UG_VALU, CK_DPII,CK_MAGIC,                        KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, UG_NEXT, UG_HUED, UG_VALD, CK_DPID, HF_TOGG,                      TO(_G0), TO(_G1), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------'                    `--------+--------+--------+--------+--------+--------|
                        _______, _______, _______,                                        _______, _______, _______,
  //                  `--------+--------+--------+-----------------`  '-----------------+--------+--------+--------'
                                          _______, _______, _______,    _______, _______, _______
  //                                    `--------------------------'  `--------------------------'
  ),

  /* Generic game */
    [_G0] = LAYOUT_2_9_regular(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,  KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,                         KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LALT, KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,                         KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,                         KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,
  //|--------+--------+--------+--------+--------+--------'                    `--------+--------+--------+--------+--------+--------|
                        _______, _______, _______,                                        _______, _______, _______,
  //                  `--------+--------+--------+-----------------`  '-----------------+--------+--------+--------'
                                           KC_SPC,  KC_SPC, MS_BTN3,    XXXXXXX, TG(_G0), XXXXXXX
  //                                    `--------------------------'  `--------------------------'
  ),

  /* Warframe */
    [_G1] = LAYOUT_2_9_regular(
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_TAB,  KC_ESC,    KC_Q,    KC_W,    KC_1,    KC_2,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         KC_F, KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_3,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LCTL,    KC_M,    KC_Y,    KC_X,    KC_4,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------'                    `--------+--------+--------+--------+--------+--------|
                        _______, _______, _______,                                        _______, _______, _______,
  //                  `--------+--------+--------+-----------------`  '-----------------+--------+--------+--------'
                                           KC_SPC,CK_RKJMP,    KC_5,    XXXXXXX, TG(_G1), XXXXXXX
  //                                    `--------------------------'  `--------------------------'
  ),
};

/* MAGIC pointing (Manual And Gaze Input Cascaded, Zhai et al. 1999), after
 * the elies keymap: while on, F19 is held down while a finger rests on the pad
 * (trackpad_rest_user: still for 60 ms, not a tap-drag), and the host pulls
 * the cursor to the gaze while it is down; moving or lifting the finger
 * releases it. CK_MAGIC (ADJUST) toggles it; off at power-up.
 *
 * Clicks while on: a long press never right clicks. A tap clicks where the
 * cursor is, unless the host moved the cursor during the touch: it then sends
 * Raw HID report 'M' (raw_hid_receive), and the touch clicks nothing
 * (trackpad_touch_no_clicks). Only a touch that rested with MAGIC on takes it.
 *
 * Split: the pad and its driver are on the right half, the keymap and Raw HID
 * on the USB half (the left one on this board). The USB half sends MAGIC and
 * the host's 'M' to the pad's half and reads back whether a MAGIC rest is on
 * (RPC_ID_USER_MAGIC, config.h); the other half also needs MAGIC for its
 * screen. Request byte: MAGIC_REQ_ON | MAGIC_REQ_NO_CLICK; answer: F19 down. */
#define MAGIC_POLL 10       /* ms between reads of a remote pad while MAGIC is on */
#define MAGIC_RESEND 500    /* ms between sends of MAGIC to a pad-less half */
#define MAGIC_CMD 'M'       /* Raw HID: the host moved the cursor during this touch */
#define MAGIC_REQ_ON 0x01
#define MAGIC_REQ_NO_CLICK 0x02

static bool magic_pointing;  /* USB half: the toggle */
static bool magic_remote;    /* other half: the toggle as last sent */
static bool magic_down;      /* USB half: F19 is down */
static bool magic_no_click;  /* USB half: the host's 'M' waits to reach the pad */
static bool pad_resting;     /* pad half: between trackpad_rest_user(true) and (false) */
static bool pad_magic_touch; /* pad half: the current touch rested with MAGIC on */
static bool pad_jump_remote; /* USB half: F19 wanted by the other half */

static bool pad_here(void) {
    return !is_keyboard_left(); /* POINTING_DEVICE_RIGHT */
}

static bool magic_on(void) {
    return is_keyboard_master() ? magic_pointing : magic_remote;
}

void trackpad_rest_user(bool resting) {
    pad_resting = resting;
    if (resting && magic_on()) {
        pad_magic_touch = true;
    }
}

bool trackpad_long_press_user(void) {
    return !magic_on();
}

/* Pad half */
static bool pad_jump(void) {
    return pad_resting && magic_on();
}

static void pad_no_click(void) {
    if (pad_magic_touch && trackpad_last_touch().touching) {
        trackpad_touch_no_clicks();
    }
}

static void magic_pad_task(void) {
    if (!trackpad_last_touch().touching) {
        pad_magic_touch = false;
    }
}

static void magic_rpc(uint8_t in_len, const void *in, uint8_t out_len, void *out) {
    if (in_len >= 1) {
        uint8_t req  = *(const uint8_t *)in;
        magic_remote = req & MAGIC_REQ_ON;
        if (req & MAGIC_REQ_NO_CLICK) {
            pad_no_click();
        }
    }
    if (out_len >= 1) {
        *(uint8_t *)out = pad_jump();
    }
}

/* Raw HID, USB half: 'M' for MAGIC, 'P' for the PC status (common/pcstatus.c) */
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length >= 1 && data[0] == MAGIC_CMD) {
        magic_no_click = magic_pointing;
        return;
    }
    pcstatus_receive(data, length);
}

/* USB half: keep the other half in step, pass on 'M' at once, press or
 * release F19 */
static void magic_task(void) {
    static bool     sent;
    static uint16_t sent_at;
    if (!is_keyboard_master()) {
        return;
    }
    if (magic_no_click && pad_here()) {
        pad_no_click();
        magic_no_click = false;
    }
    uint16_t period = pad_here() ? MAGIC_RESEND : MAGIC_POLL;
    if (sent != magic_pointing || magic_no_click || (magic_pointing && timer_elapsed(sent_at) >= period)) {
        uint8_t in  = (magic_pointing ? MAGIC_REQ_ON : 0) | (magic_no_click ? MAGIC_REQ_NO_CLICK : 0);
        uint8_t out = 0;
        if (transaction_rpc_exec(RPC_ID_USER_MAGIC, sizeof(in), &in, sizeof(out), &out)) {
            sent            = magic_pointing;
            magic_no_click  = false;
            pad_jump_remote = out;
        } else {
            pad_jump_remote = false;
        }
        sent_at = timer_read();
    }
    bool down = magic_pointing && (pad_here() ? pad_jump() : pad_jump_remote);
    if (down != magic_down) {
        if (down) {
            register_code(KC_F19);
        } else {
            unregister_code(KC_F19);
        }
        magic_down = down;
    }
}

void keyboard_post_init_user(void) {
    accents_init();
    transaction_register_rpc(RPC_ID_USER_MAGIC, magic_rpc);
}

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
            case CK_OSMD: /* Cycle accent OS mode */
                if (record->event.pressed) {
                    os_mode_cycle();
                }
                return false;
            case CK_MAGIC: /* MAGIC pointing on/off */
                if (record->event.pressed) {
                    magic_pointing = !magic_pointing;
                    magic_no_click = false;
                }
                return false;
        }
    }

    /* Accented letters */
    if(accent_state != ACCENT_NONE
    && record->event.pressed)
    {
        switch (keycode) {
            case KC_A:
                switch (accent_state) {
                    case ACCENT_LEFT:
                        send_accent(ACC_A_GRAVE);
                        break;
                    case ACCENT_RIGHT:
                        send_accent(ACC_A_CIRCU);
                        break;
                    default:
                        break;
                }
                break;
            case KC_C:
                send_accent(ACC_C_CEDIL);
                break;
            case KC_E:
                switch (accent_state) {
                    case ACCENT_LEFT:
                        send_accent(ACC_E_ACUTE);
                        break;
                    case ACCENT_RIGHT:
                        send_accent(ACC_E_GRAVE);
                        break;
                    case ACCENT_SUPER_LEFT:
                        send_accent(ACC_E_CIRCU);
                        break;
                    case ACCENT_SUPER_RIGHT:
                        send_accent(ACC_EURO);
                        break;
                    default:
                        break;
                }
                break;
            case KC_I:
                switch (accent_state) {
                    case ACCENT_LEFT:
                        send_accent(ACC_I_TREMA);
                        break;
                    case ACCENT_RIGHT:
                        send_accent(ACC_I_CIRCU);
                        break;
                    default:
                        break;
                }
                break;
            case KC_O:
                switch (accent_state) {
                    case ACCENT_LEFT:
                        send_accent(ACC_O_TREMA);
                        break;
                    case ACCENT_RIGHT:
                        send_accent(ACC_O_CIRCU);
                        break;
                    default:
                        break;
                }
                break;
            case KC_U:
                switch (accent_state) {
                    case ACCENT_LEFT:
                        send_accent(ACC_U_GRAVE);
                        break;
                    case ACCENT_RIGHT:
                        send_accent(ACC_U_CIRCU);
                        break;
                    default:
                        break;
                }
                break;
        }
        if(keycode >= KC_A && keycode <= KC_Z)
        {
            accent_state = ACCENT_NONE;
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
    /* Landscape on both halves (left 128x64, right 128x32, see v2_9/v2_9.c) */
    return OLED_ROTATION_180;
}

/* Write text at (col, line), padded with spaces to width, centred if asked */
static void oled_write_field(uint8_t col, uint8_t line, uint8_t width, const char *text, bool center) {
    char   buf[22];
    size_t len = strlen(text);
    if (width > sizeof(buf) - 1) width = sizeof(buf) - 1;
    if (len > width) len = width;
    uint8_t pad = center ? (width - len) / 2 : 0;
    memset(buf, ' ', width);
    memcpy(buf + pad, text, len);
    buf[width] = '\0';
    oled_set_cursor(col, line);
    oled_write(buf, false);
}

/* Layer names, 3 letters on both screens */
static const char *layer_label(void) {
    switch (get_highest_layer(layer_state)) {
        case _ALPHA:  return "ABC";
        case _NAV:    return "NAV";
        case _NUM:    return "NUM";
        case _ADJUST: return "ADJ";
        case _G0:     return "GM0";
        case _G1:     return "GM1";
        default:      return "???";
    }
}

/* Landscape, 21 columns: logo in columns 0-5, text from column 6 on the
 * logo's 4 lines (centred vertically on the taller left screen)
 *   +0  layer               +2  DPI (ADJUST)
 *   +1  LNX / WIN (USB half) +3  MAGIC (MAGIC pointing on)
 * The PC status screen (left) shows MAGIC on its free bottom line. */
bool oled_task_user(void) {
    uint8_t top = chocozor_logo_top();
    char    dpi[16] = "";

    /* Left screen: the PC status while the host helper sends it, else the
     * same block as the right; clear when switching between the two */
    static bool showing_status;
    bool        status = is_keyboard_left() && pcstatus_active();
    if (status != showing_status) {
        oled_clear();
        showing_status = status;
    }
    if (status) {
        pcstatus_render();
        oled_write_field(0, 7, 21, magic_on() ? "MAGIC" : "", false);
        return false;
    }

    chocozor_render_logo();

    if (get_highest_layer(layer_state) == _ADJUST) {
        snprintf(dpi, sizeof(dpi), "DPI %u", pointing_device_get_cpi());
    }
    oled_write_field(6, top + 0, 15, layer_label(), false);
    oled_write_field(6, top + 1, 15, os_mode_label(), false);
    oled_write_field(6, top + 2, 15, dpi, false);
    oled_write_field(6, top + 3, 15, magic_on() ? "MAGIC" : "", false);

    return false;
}

void pointing_device_init_user(void) {
    pointing_device_set_cpi(TRACKPAD_DEFAULT_DPI);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    SEND_STRING(SS_UP(X_LALT));
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case _ALPHA:
            if (index == 0) { tap_code((clockwise ? KC_DOWN : KC_UP)); }
            else if (index == 1) { tap_code((clockwise ? KC_RIGHT : KC_LEFT)); }
            break;
        case _NAV:
        case _NUM:
        case _ADJUST:
        case _G0:
        case _G1:
            if (index == 0) { tap_code((clockwise ? KC_VOLU : KC_VOLD)); }
            else if (index == 1) {
                /* - Hold Alt, it will be released at next layer switch in `layer_state_set_user()` */
                SEND_STRING(SS_DOWN(X_LALT));
                if (clockwise) { tap_code(KC_TAB); }
                else { SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_TAB) SS_UP(X_LSFT)); }
            }
    }
    return false;
}

bool apply_autocorrect(uint8_t backspaces, const char *str, char *typo, char *correct) {
    #ifdef RGB_MATRIX_ENABLE
    if(rgb_matrix_is_enabled())
    {
        previous_rgb_matrix_mode = rgb_matrix_get_mode();
        previous_rgb_matrix_hsv = rgb_matrix_get_hsv();
        rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
        rgb_matrix_sethsv(HSV_MAGENTA);
        check_rgblight_timer = timer_read();
    }
    #endif // RGB_MATRIX_ENABLE
    solenoid_fire(0);
    return true;
}

void housekeeping_task_user(void) {
    accents_sync_task();
    if (pad_here()) {
        magic_pad_task();
    }
    magic_task();
    #ifdef RGB_MATRIX_ENABLE
    /* Reset RGB Matrix autocorrect indicator */
    if (previous_rgb_matrix_mode != RGB_MATRIX_NONE && timer_elapsed(check_rgblight_timer) > 200) {
        rgb_matrix_mode(previous_rgb_matrix_mode);
        rgb_matrix_sethsv(previous_rgb_matrix_hsv.h, previous_rgb_matrix_hsv.s, previous_rgb_matrix_hsv.v);
        previous_rgb_matrix_mode = RGB_MATRIX_NONE;
        previous_rgb_matrix_hsv = (hsv_t){HSV_OFF};
        check_rgblight_timer = 0;
    }
    #endif // RGB_MATRIX_ENABLE
}

bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    //switch (keycode) {
    //    case KC_ESC:
    //        return true;
    //        break;
    //}
    return false;
}
