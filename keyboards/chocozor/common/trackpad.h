#pragma once

#include <stdbool.h>
#include <stdint.h>

/* The last touch the pad saw, in pad units (0-255 across, y downwards) and
 * the chip's touch strength. Only meaningful on the half with the pad. */
typedef struct {
    uint8_t x, y, z;
    bool    touching;
} trackpad_touch_t;

trackpad_touch_t trackpad_last_touch(void);

/* A long press happened (still down); return true for the default right click. */
bool trackpad_long_press_user(void);

/* A touch started or stopped resting: still, past the point where it could be
 * a tap (see trackpad.c). Every true is followed by a false. */
void trackpad_rest_user(bool resting);

/* The current touch only moves the cursor: its tap or long press clicks nothing. */
void trackpad_touch_no_clicks(void);
