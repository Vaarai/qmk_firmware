/* Copyright 2024 Vaarai
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
/* The halves have different screens (config.h): the build is for the left's
 * 128x64, and the right half, told apart by the handedness pin, sets its
 * 128x32 panel to 32 rows after QMK's setup. It then shows pages 0-3 of the
 * buffer, so it draws on 4 lines (chocozor_oled_lines). */
#    define SSD1306_CMD 0x00
#    define SSD1306_MULTIPLEX_RATIO 0xA8
#    define SSD1306_COM_PINS 0xDA
#    define SSD1306_COM_PINS_SEQ 0x02

uint8_t chocozor_oled_lines(void) {
    return is_keyboard_left() ? oled_max_lines() : 4;
}

void keyboard_post_init_kb(void) {
    if (!is_keyboard_left()) {
        static const uint8_t panel_128x32[] = {SSD1306_CMD, SSD1306_MULTIPLEX_RATIO, 32 - 1, SSD1306_COM_PINS, SSD1306_COM_PINS_SEQ};
        oled_send_cmd(panel_128x32, sizeof(panel_128x32));
    }
    keyboard_post_init_user();
}
#endif
