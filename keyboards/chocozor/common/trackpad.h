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

/* Called when a finger lands on the pad (not while it stays down, nor when the
 * touch continues a tap into a drag). */
void trackpad_touch_start_user(void);

/* The current touch only moves the cursor: its tap or long press clicks nothing. */
void trackpad_touch_no_clicks(void);
