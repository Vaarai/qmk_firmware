/* Raw HID protocol for the keyhud overlay (~/.dots/modules/shared/ui/keyhud).
 *
 * Host -> board:
 *   [HUD_HELLO]                 keeps the state stream on for HUD_ALIVE_MS,
 *                               answered by [HUD_HELLO, layers, rows, cols,
 *                               encoders]
 *   [HUD_KEYMAP, layer, index]  answered by [HUD_KEYMAP, layer, index, count,
 *                               keycodes (u16 LE)...] from matrix index on
 *   [HUD_ENCODERS, layer]       answered by [HUD_ENCODERS, layer, count,
 *                               (ccw, cw) keycode pairs (u16 LE)...]
 *   [HUD_TOUCH]                 answered by [HUD_TOUCH, x, y, z, touching], the
 *                               pad's last touch (common/trackpad.h), when this half
 *                               has the pad
 *   [HUD_BOOT, 'B', 'O', 'O', 'T'] reboots into the bootloader (flash.py)
 * Board -> host, on any change while the stream is on:
 *   [HUD_STATE, layer_state (u32 LE), default_layer_state (u32 LE), mods,
 *    oneshot mods, caps word, rows, one matrix row bitmask per row...]
 *
 * Nothing is sent unless a host said hello recently: a report nobody reads
 * would stall the keyboard on its USB timeout. */

#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "keymap_introspection.h"
#include "../../common/trackpad.h"

enum { HUD_HELLO = 1, HUD_KEYMAP = 2, HUD_STATE = 3, HUD_BOOT = 4, HUD_ENCODERS = 5, HUD_TOUCH = 6 };

#define HUD_REPORT 32
#define HUD_ALIVE_MS 3000
#define HUD_KEYS (MATRIX_ROWS * MATRIX_COLS)

_Static_assert(MATRIX_COLS <= 8, "one byte per matrix row");
_Static_assert(3 + 4 * NUM_ENCODERS <= HUD_REPORT, "encoder report too long");
_Static_assert(12 + MATRIX_ROWS <= HUD_REPORT, "state report too long");

static bool     hud_alive;
static uint32_t hud_ping;
static uint8_t  hud_last[HUD_REPORT];

static void hud_put32(uint8_t *at, uint32_t v) {
    for (uint8_t i = 0; i < 4; i++) {
        at[i] = v >> (8 * i);
    }
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    uint8_t out[HUD_REPORT] = {0};
    out[0]                  = data[0];
    switch (data[0]) {
        case HUD_HELLO:
            if (!hud_alive) {
                memset(hud_last, 0, sizeof(hud_last));
            }
            hud_alive = true;
            hud_ping  = timer_read32();
            out[1]    = keymap_layer_count();
            out[2]    = MATRIX_ROWS;
            out[3]    = MATRIX_COLS;
            out[4]    = NUM_ENCODERS;
            break;
        case HUD_KEYMAP: {
            uint8_t layer = data[1], index = data[2], count = 0;
            out[1]        = layer;
            out[2]        = index;
            if (layer < keymap_layer_count()) {
                for (; count < (HUD_REPORT - 4) / 2 && index + count < HUD_KEYS; count++) {
                    uint8_t  i  = index + count;
                    uint16_t kc = keycode_at_keymap_location(layer, i / MATRIX_COLS, i % MATRIX_COLS);
                    out[4 + 2 * count] = kc & 0xFF;
                    out[5 + 2 * count] = kc >> 8;
                }
            }
            out[3] = count;
            break;
        }
        case HUD_ENCODERS: {
            uint8_t layer = data[1];
            out[1]        = layer;
            if (layer < encodermap_layer_count()) {
                out[2] = NUM_ENCODERS;
                for (uint8_t e = 0; e < NUM_ENCODERS; e++) {
                    for (uint8_t cw = 0; cw < 2; cw++) {
                        uint16_t kc             = keycode_at_encodermap_location(layer, e, cw);
                        out[3 + 4 * e + 2 * cw] = kc & 0xFF;
                        out[4 + 4 * e + 2 * cw] = kc >> 8;
                    }
                }
            }
            break;
        }
        case HUD_TOUCH: {
            trackpad_touch_t t = trackpad_last_touch();
            out[1]             = t.x;
            out[2]             = t.y;
            out[3]             = t.z;
            out[4]             = t.touching;
            break;
        }
        case HUD_BOOT:
            if (memcmp(&data[1], "BOOT", 4) == 0) {
                reset_keyboard();
            }
            return;
        default:
            return;
    }
    raw_hid_send(out, sizeof(out));
}

void housekeeping_task_user(void) {
    if (!hud_alive) {
        return;
    }
    if (timer_elapsed32(hud_ping) > HUD_ALIVE_MS) {
        hud_alive = false;
        return;
    }
    uint8_t out[HUD_REPORT] = {HUD_STATE};
    hud_put32(&out[1], layer_state);
    hud_put32(&out[5], default_layer_state);
    out[9]  = get_mods();
    out[10] = get_oneshot_mods();
    out[11] = is_caps_word_on();
    out[12] = MATRIX_ROWS;
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        out[13 + r] = matrix_get_row(r);
    }
    if (memcmp(out, hud_last, sizeof(out)) != 0) {
        memcpy(hud_last, out, sizeof(out));
        raw_hid_send(out, sizeof(out));
    }
}
