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

// ============ I²C config =============
#define I2C_DRIVER I2CD0
#define I2C1_SDA_PIN GP12
#define I2C1_SCL_PIN GP13

#undef SOFT_SERIAL_PIN
#define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode.
#define SERIAL_USART_TX_PIN GP0     // USART TX pin
#define SERIAL_USART_RX_PIN GP1     // USART RX pin

// ============ Handedness  =============
//#define	SPLIT_HAND_PIN_LOW_IS_LEFT

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 400U // Timeout window in ms in which the double tap can occur.

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 128 // limits maximum brightness of LEDs to 200 out of 255. If not defined maximum brightness is set to 255
#define RGB_MATRIX_DEFAULT_ON false // Sets the default enabled state, if none has been set
#define RGB_MATRIX_DEFAULT_HUE 0 // Sets the default hue value, if none has been set
#define RGB_MATRIX_DEFAULT_SAT 10 // Sets the default saturation value, if none has been set
#define RGB_MATRIX_DEFAULT_VAL 20 // Sets the default brightness value, if none has been set

// ======= Solenoid =====
#define SOLENOID_COUNT 1
#define SOLENOID_PIN GP25
#define SOLENOID_DEFAULT_DWELL 4
#define SOLENOID_MIN_DWELL 1
#define SOLENOID_MAX_DWELL 15
#define SOLENOID_DWELL_STEP_SIZE 1
#define HAPTIC_OFF_IN_LOW_POWER 1
#define NO_HAPTIC_MOD
// #define SOLENOID_PIN_ACTIVE_LOW