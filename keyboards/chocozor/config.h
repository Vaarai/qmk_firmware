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

/* Split */
#define SPLIT_ACTIVITY_ENABLE
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE

/* Trackpad: a 40 mm Cirque Pinnacle on the right half, driven by
 * common/trackpad.c (common/trackpad.mk); its bus is set per revision. */
#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_RIGHT
#define CIRQUE_PINNACLE_DIAMETER_MM 40
#define CIRQUE_PINNACLE_ATTENUATION EXTREG__TRACK_ADCCONFIG__ADC_ATTENUATE_4X
/* Absolute mode: the taps are ours (trackpad.c), not the chip's */
#define CIRQUE_PINNACLE_POSITION_MODE CIRQUE_PINNACLE_ABSOLUTE_MODE
#define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE
/* Holding this layer turns trackpad motion into scrolling (_NAV) */
#define TRACKPAD_SCROLL_LAYER 1
