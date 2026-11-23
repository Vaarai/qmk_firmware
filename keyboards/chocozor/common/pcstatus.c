/* Copyright 2024 Vaarai
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * PC status helper: the host (~/.dots chocozor-status) sends one 32-byte
 * Raw HID report per second, the keyboard only draws it and never answers
 * (a report nobody reads would stall the board on its USB timeout).
 *
 * Report, all bytes unsigned, 0xFF / 0xFFFF = not available:
 *   [0]  PCSTATUS_CMD ('P')   [1]  version (1)
 *   [2]  hour  [3] minute  [4] second
 *   [5]  weekday (0 = Monday)  [6] day  [7] month
 *   [8]  CPU %  [9] RAM %  [10] CPU temp C  [11] GPU temp C
 *   [12-13] notifications, little endian
 * The keymap's raw_hid_receive passes reports on to pcstatus_receive, which
 * ignores other ids (e.g. the keyhud protocol, ids 1-9, MAGIC pointing 'M').
 * The GPU line is off unless PCSTATUS_GPU is defined (config.h).
 *
 * Split: the status is drawn on the left screen. When the right half is the
 * USB one, it passes each report on to the left half (RPC_ID_USER_PCSTATUS,
 * to list in the keymap's SPLIT_TRANSACTION_IDS_USER), which then times it
 * out on its own. Call pcstatus_init from keyboard_post_init_user and
 * pcstatus_sync_task from housekeeping_task_user. */

#include "pcstatus.h"
#include <stdio.h>
#include <string.h>
#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif

#define PCSTATUS_CMD 'P'
#define PCSTATUS_VERSION 1
#define PCSTATUS_TIMEOUT 3000 /* ms without a report before the screen shows dashes */
#define NA8 0xFF

typedef struct {
    uint8_t  hour, minute, second;
    uint8_t  weekday, day, month;
    uint8_t  cpu, ram, cpu_temp, gpu_temp;
    uint16_t notifications;
} pcstatus_t;

static pcstatus_t status;
static uint32_t   received_at;
static bool       received;
static bool       pending; /* USB half: a report not yet passed to the screen half */

bool pcstatus_receive(const uint8_t *data, uint8_t length) {
    if (length < 14 || data[0] != PCSTATUS_CMD || data[1] != PCSTATUS_VERSION) {
        return false;
    }
    status = (pcstatus_t){
        .hour          = data[2],
        .minute        = data[3],
        .second        = data[4],
        .weekday       = data[5],
        .day           = data[6],
        .month         = data[7],
        .cpu           = data[8],
        .ram           = data[9],
        .cpu_temp      = data[10],
        .gpu_temp      = data[11],
        .notifications = data[12] | (data[13] << 8),
    };
    received_at = timer_read32();
    received    = true;
    pending     = true;
    return true;
}

#ifdef SPLIT_KEYBOARD
/* Screen half: a report from the USB half */
static void pcstatus_rpc(uint8_t in_len, const void *in, uint8_t out_len, void *out) {
    if (in_len == sizeof(status)) {
        memcpy(&status, in, sizeof(status));
        received_at = timer_read32();
        received    = true;
    }
}

void pcstatus_init(void) {
    transaction_register_rpc(RPC_ID_USER_PCSTATUS, pcstatus_rpc);
}

/* USB half, if it isn't the screen half: pass the last report on, once,
 * retried on the next loop if the transfer fails */
void pcstatus_sync_task(void) {
    if (!pending || !is_keyboard_master()) {
        return;
    }
    if (is_keyboard_left() || transaction_rpc_send(RPC_ID_USER_PCSTATUS, sizeof(status), &status)) {
        pending = false;
    }
}
#else
void pcstatus_init(void) {}
void pcstatus_sync_task(void) {}
#endif

bool pcstatus_active(void) {
    return received && timer_elapsed32(received_at) < PCSTATUS_TIMEOUT;
}

#ifdef OLED_ENABLE
/* A bar of width w pixels at (x, line), filled to percent */
static void draw_bar(uint8_t x, uint8_t line, uint8_t w, uint8_t percent) {
    uint8_t y0 = line * 8 + 1, y1 = line * 8 + 6;
    uint8_t fill = percent > 100 ? w : (uint16_t)w * percent / 100;
    for (uint8_t i = 0; i < w; i++) {
        for (uint8_t y = y0; y <= y1; y++) {
            bool edge = (i == 0 || i == w - 1 || y == y0 || y == y1);
            oled_write_pixel(x + i, y, edge || i < fill);
        }
    }
}

static void write_line(uint8_t line, const char *text) {
    char buf[22];
    snprintf(buf, sizeof(buf), "%-21s", text);
    oled_set_cursor(0, line);
    oled_write(buf, false);
}

/* Write text at (col, line), padded with spaces to width columns */
static void write_at(uint8_t col, uint8_t line, const char *text, uint8_t width) {
    char buf[22];
    snprintf(buf, sizeof(buf), "%-*.*s", width, width, text);
    oled_set_cursor(col, line);
    oled_write(buf, false);
}

static void temp_text(char *buf, size_t size, uint8_t temp) {
    if (temp == NA8) {
        snprintf(buf, size, " --");
    } else {
        snprintf(buf, size, "%3uC", temp);
    }
}

static void pct_text(char *buf, size_t size, const char *name, uint8_t pct) {
    if (pct == NA8) {
        snprintf(buf, size, "%s  --%% ", name);
    } else {
        snprintf(buf, size, "%s %3u%% ", name, pct);
    }
}

/* QMK's screen font is private to its driver: a copy of it for the big clock */
#include OLED_FONT_H

/* Text in double size (12 x 16 pixels a character) from pixel column x on
 * text lines line and line + 1 */
static void write_big(uint8_t x, uint8_t line, const char *text) {
    for (; *text; text++, x += OLED_FONT_WIDTH * 2) {
        const unsigned char *glyph = &font[(uint8_t)*text * OLED_FONT_WIDTH];
        for (uint8_t col = 0; col < OLED_FONT_WIDTH; col++) {
            uint8_t bits = pgm_read_byte(&glyph[col]);
            for (uint8_t row = 0; row < 8; row++) {
                bool    on = bits & (1 << row);
                uint8_t px = x + col * 2, py = line * 8 + row * 2;
                oled_write_pixel(px, py, on);
                oled_write_pixel(px + 1, py, on);
                oled_write_pixel(px, py + 1, on);
                oled_write_pixel(px + 1, py + 1, on);
            }
        }
    }
}

/* Pixel columns [x0, x1) of text lines line and line + 1 off */
static void clear_big(uint8_t x0, uint8_t x1, uint8_t line) {
    for (uint8_t x = x0; x < x1; x++) {
        for (uint8_t y = line * 8; y < line * 8 + 16; y++) {
            oled_write_pixel(x, y, false);
        }
    }
}

/*  0-1  14:32:05            (double size, centred)
 *  2        Thu 20 Nov      (centred)
 *  3    GPU             47C (PCSTATUS_GPU builds, with a GPU sensor)
 *  4    CPU  23% [=====   ]  52C
 *  5    RAM  41% [========]
 *  6    Notifications       3
 *  7    "Waiting for the PC" while no status comes (dashes elsewhere) */
void pcstatus_render(void) {
    static const char *const days[]   = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    static const char *const months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char line[32], temp[8];

    /* No report for PCSTATUS_TIMEOUT: every field unknown */
    bool waiting = !pcstatus_active();
    if (waiting) {
        memset(&status, NA8, sizeof(status));
    }

    /* "hh:mm:ss" is 8 x 12 = 96 pixels: 16 free on each side */
    if (status.hour == NA8) {
        snprintf(line, sizeof(line), "--:--:--");
    } else {
        snprintf(line, sizeof(line), "%02u:%02u:%02u", status.hour, status.minute, status.second);
    }
    clear_big(0, 16, 0);
    write_big(16, 0, line);
    clear_big(112, 128, 0);

    if (status.hour == NA8) {
        snprintf(line, sizeof(line), "%15s", "--- -- ---");
    } else {
        snprintf(line, sizeof(line), "     %s %2u %s", status.weekday < 7 ? days[status.weekday] : "---", status.day,
                 status.month >= 1 && status.month <= 12 ? months[status.month - 1] : "---");
    }
    write_line(2, line);

#ifdef PCSTATUS_GPU
    /* GPU only when the host has a sensor for it */
    if (status.gpu_temp == NA8) {
        write_line(3, "");
    } else {
        temp_text(temp, sizeof(temp), status.gpu_temp);
        snprintf(line, sizeof(line), "GPU              %4s", temp);
        write_line(3, line);
    }
#else
    write_line(3, "");
#endif

    /* "CPU  23% " in columns 0-8, bar on pixels 54-95 (columns 9-15, never
     * written as text so it isn't redrawn every frame), temperature in 16-20 */
    temp_text(temp, sizeof(temp), status.cpu_temp);
    pct_text(line, sizeof(line), "CPU", status.cpu);
    write_at(0, 4, line, 9);
    snprintf(line, sizeof(line), " %4s", temp);
    write_at(16, 4, line, 5);
    pct_text(line, sizeof(line), "RAM", status.ram);
    write_at(0, 5, line, 9);
    write_at(16, 5, "", 5);
    draw_bar(54, 4, 42, status.cpu == NA8 ? 0 : status.cpu);
    draw_bar(54, 5, 42, status.ram == NA8 ? 0 : status.ram);

    if (status.notifications == 0xFFFF) {
        write_line(6, "Notifications     --");
    } else {
        snprintf(line, sizeof(line), "Notifications  %5u", status.notifications);
        write_line(6, line);
    }
    write_line(7, waiting ? "Waiting for the PC" : "");
}
#endif
