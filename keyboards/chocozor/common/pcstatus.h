/* Copyright 2024 Vaarai
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include QMK_KEYBOARD_H

/* PC status sent by the host helper over Raw HID (common/pcstatus.c) */

/* True while the host has sent a status in the last PCSTATUS_TIMEOUT ms */
bool pcstatus_active(void);
/* Draw the status on the whole screen (landscape, 21 x 8 or more lines) */
void pcstatus_render(void);
