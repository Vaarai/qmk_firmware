/* Trackpad: the Cirque Pinnacle in absolute mode, with our own gestures since
 * neither the chip nor QMK's driver has tap-and-drag. This is the pointing
 * device driver, so it runs on the right half, where the pad is.
 *
 * - one finger moves the cursor, and a flick glides on (QMK's cursor glide)
 * - tap: left click. The button stays down for TP_DRAG_GAP after the tap, so
 *   touching again in that time drags until the finger lifts; tapping again
 *   instead makes a double click, so the cursor holds still during that second
 *   touch until it moves or lasts too long for a tap
 * - tap in the top-right corner: right click; top-left corner: middle click
 * - long press: a fresh touch held still for TP_LONG_PRESS right clicks, while
 *   still down. Only from touchdown: a finger that moved, then paused to adjust,
 *   never right clicks; lifting and touching again does. Nor does a touch soon
 *   after a tap (TP_AFTER_TAP), even past the drag gap: that's a slow tap-drag
 *   pausing before it moves
 * Holding NAV (TRACKPAD_SCROLL_LAYER, config.h) turns motion into scrolling
 * (pointing_device_task_user). Each fresh touch (not one continuing a tap into
 * a drag) calls trackpad_touch_start_user (weak, for keymaps);
 * trackpad_touch_no_clicks makes the current touch only move: no tap, no long
 * press. Bus-agnostic: common/trackpad.mk links the
 * Cirque core over I2C or SPI. */

#include QMK_KEYBOARD_H
#include "trackpad.h"

#define TP_TAP_TERM 180 /* ms: longer touches aren't taps */
#define TP_TAP_SLOP 10  /* pad units: taps move less than this */
#define TP_DRAG_GAP 220 /* ms: a touch this soon after a tap drags */
#define TP_LONG_PRESS 500 /* ms a fresh touch stays still to right click */
#define TP_LONG_SLOP 3    /* pad units: a long press moves less than this, the
                           * sensor's jitter but no deliberate motion */
#define TP_AFTER_TAP 600  /* ms after a tap during which a touch can't long press */
#define TP_CORNER 72    /* pad units: size of the corners that right and middle click */
#define TP_SCROLL_DIV 24 /* cursor counts per scroll step with NAV held */
#define TP_CLICK 30      /* ms a click holds its button, and the gap before a
                          * click right after a release: libinput drops a
                          * release and press closer than ~12 ms as bounce */

#define TP_BUTTONS (MOUSE_BTN1 | MOUSE_BTN2 | MOUSE_BTN3)

static struct {
    bool     touching;
    bool     tapped; /* a tap's left button is down, waiting for TP_DRAG_GAP */
    bool     drag;   /* this touch started while tapped */
    bool     pressed; /* this touch long pressed, or may not */
    bool     quiet;   /* this touch only moves (trackpad_touch_no_clicks) */
    uint16_t tap_end; /* when the last tap lifted */
    uint16_t since;  /* touch start, or tap end while tapped */
    uint8_t  x0, y0, travel;
    uint16_t sx, sy; /* last scaled position */
    uint8_t  held;   /* buttons down until we let go */
    uint8_t  click;    /* buttons of a click under way */
    uint16_t click_at; /* when it presses; it releases TP_CLICK later */
} tp;

static trackpad_touch_t       last;
static cursor_glide_context_t glide = {.config = {.coef = 102, .interval = 10, .trigger_px = 10}};

trackpad_touch_t trackpad_last_touch(void) {
    return last;
}

__attribute__((weak)) void trackpad_touch_start_user(void) {}

void trackpad_touch_no_clicks(void) {
    if (tp.touching) {
        tp.quiet   = true;
        tp.pressed = true;
    }
}

static uint8_t pad_unit(uint16_t v, uint16_t lower, uint16_t range) {
    v = v < lower ? 0 : v - lower;
    return (v > range ? range : v) * 255UL / range;
}

static uint8_t corner_button(uint8_t x, uint8_t y) {
    if (y > TP_CORNER) {
        return 0;
    }
    return x >= 255 - TP_CORNER ? MOUSE_BTN2 : x <= TP_CORNER ? MOUSE_BTN3 : 0;
}

bool pointing_device_driver_init(void) {
    return cirque_pinnacle_init();
}

uint16_t pointing_device_driver_get_cpi(void) {
    return cirque_pinnacle_get_cpi();
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    cirque_pinnacle_set_cpi(cpi);
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t report) {
    pinnacle_data_t d     = cirque_pinnacle_read_data();
    cursor_glide_t  glided = cursor_glide_check(&glide);
    int16_t         dx = 0, dy = 0;

    if (tp.tapped && !tp.touching && timer_elapsed(tp.since) > TP_DRAG_GAP) {
        tp.tapped = false;
        tp.held &= ~MOUSE_BTN1;
    }

    if (d.valid && d.touchDown) {
        uint8_t ux = pad_unit(d.xValue, CIRQUE_PINNACLE_X_LOWER, CIRQUE_PINNACLE_X_RANGE);
        uint8_t uy = pad_unit(d.yValue, CIRQUE_PINNACLE_Y_LOWER, CIRQUE_PINNACLE_Y_RANGE);
        last       = (trackpad_touch_t){ux, uy, d.zValue, true};
        cirque_pinnacle_scale_data(&d, cirque_pinnacle_get_scale(), cirque_pinnacle_get_scale());
        if (!tp.touching) {
            tp.touching = true;
            tp.drag     = tp.tapped;
            tp.pressed  = tp.drag || timer_elapsed(tp.tap_end) < TP_AFTER_TAP;
            tp.tapped   = false;
            tp.since    = timer_read();
            tp.x0       = ux;
            tp.y0       = uy;
            tp.travel   = 0;
            tp.quiet    = false;
            if (!tp.drag) {
                trackpad_touch_start_user();
            }
        } else {
            dx = d.xValue - tp.sx;
            dy = d.yValue - tp.sy;
            uint8_t far = MAX(abs(ux - tp.x0), abs(uy - tp.y0));
            tp.travel   = MAX(tp.travel, far);
            if (tp.drag && tp.travel < TP_TAP_SLOP && timer_elapsed(tp.since) < TP_TAP_TERM) {
                dx = dy = 0; /* maybe a double tap: jitter with the button down would be a drag */
            }
            if (!tp.pressed && tp.travel >= TP_LONG_SLOP) {
                tp.pressed = true; /* it moved: not a long press, even if it stops */
            }
            if (!tp.pressed && timer_elapsed(tp.since) >= TP_LONG_PRESS) {
                tp.pressed  = true;
                tp.click    = MOUSE_BTN2;
                tp.click_at = timer_read();
            }
        }
        tp.sx = d.xValue;
        tp.sy = d.yValue;
        cursor_glide_update(&glide, CONSTRAIN_HID_XY(dx), CONSTRAIN_HID_XY(dy), d.zValue);
    } else if (d.valid && tp.touching) {
        bool tap    = !tp.quiet && timer_elapsed(tp.since) < TP_TAP_TERM && tp.travel < TP_TAP_SLOP;
        bool held   = tp.pressed && timer_elapsed(tp.since) >= TP_LONG_PRESS && tp.travel < TP_LONG_SLOP;
        tp.touching = false;
        last.touching = false;
        if (tp.drag) {
            tp.held &= ~MOUSE_BTN1;
            if (tap) {
                tp.click    = MOUSE_BTN1;
                tp.click_at = timer_read() + TP_CLICK;
            }
        } else if (tap && corner_button(tp.x0, tp.y0)) {
            tp.click    = corner_button(tp.x0, tp.y0);
            tp.click_at = timer_read();
        } else if (tap) {
            tp.held |= MOUSE_BTN1;
            tp.tapped = true;
            tp.since  = timer_read();
        }
        if (tap) {
            tp.tap_end = timer_read();
        }
        if (!tap && !held) {
            glided = cursor_glide_start(&glide);
        } else {
            cursor_glide_update(&glide, 0, 0, 0);
            glided.valid = false;
        }
    }
    if (!tp.touching && glided.valid) {
        dx = glided.dx;
        dy = glided.dy;
    }

    report.x       = CONSTRAIN_HID_XY(dx);
    report.y       = CONSTRAIN_HID_XY(dy);
    uint8_t clicking = 0;
    if (tp.click) {
        int16_t t = (int16_t)(timer_read() - tp.click_at);
        if (t >= TP_CLICK) {
            tp.click = 0;
        } else if (t >= 0) {
            clicking = tp.click;
        }
    }
    report.buttons = (report.buttons & ~TP_BUTTONS) | tp.held | clicking;
    return report;
}

report_mouse_t pointing_device_task_user(report_mouse_t report) {
    static int16_t sx, sy;
    if (!layer_state_is(TRACKPAD_SCROLL_LAYER)) {
        sx = sy = 0;
        return report;
    }
    sx += report.x;
    sy += report.y;
    report.h = sx / TP_SCROLL_DIV;
    report.v = -(sy / TP_SCROLL_DIV);
    sx -= report.h * TP_SCROLL_DIV;
    sy += report.v * TP_SCROLL_DIV;
    report.x = report.y = 0;
    return report;
}
