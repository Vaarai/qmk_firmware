/* Copyright 2024 Vaarai
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include QMK_KEYBOARD_H

/* PC status sent by the host helper over Raw HID (common/pcstatus.c) */

/* A Raw HID report from the keymap's raw_hid_receive: true if it was a PC
 * status ('P'), false for any other report */
bool pcstatus_receive(const uint8_t *data, uint8_t length);

/* True while the host has sent a status in the last PCSTATUS_TIMEOUT ms */
bool pcstatus_active(void);
/* Draw the status on lines 0-7 (landscape, 21 x 8 lines); dashes and
 * "Waiting for the PC" while no status comes */
void pcstatus_render(void);
