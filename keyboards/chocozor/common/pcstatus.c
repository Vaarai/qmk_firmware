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
 * Other reports (e.g. the keyhud protocol, ids 1-6) are ignored.
 * The GPU line is off unless PCSTATUS_GPU is defined (config.h). */

#include "pcstatus.h"
#include "raw_hid.h"
#include <stdio.h>

#define PCSTATUS_CMD 'P'
#define PCSTATUS_VERSION 1
#define PCSTATUS_TIMEOUT 3000 /* ms without a report before the screen goes back */
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

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length < 14 || data[0] != PCSTATUS_CMD || data[1] != PCSTATUS_VERSION) {
        return;
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
}

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

/*  0  Thu 20 Nov   14:32:05
 *  2  CPU  23% [=====   ]  52C
 *  3  RAM  41% [========]
 *  4  GPU                  47C   (PCSTATUS_GPU builds, with a GPU sensor)
 *  6  Notifications       3     */
void pcstatus_render(void) {
    static const char *const days[]   = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    static const char *const months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char line[32], temp[8];

    snprintf(line, sizeof(line), "%s %2u %s   %02u:%02u:%02u", status.weekday < 7 ? days[status.weekday] : "---", status.day,
             status.month >= 1 && status.month <= 12 ? months[status.month - 1] : "---", status.hour, status.minute, status.second);
    write_line(0, line);
    write_line(1, "");

    /* "CPU  23% " in columns 0-8, bar on pixels 54-95 (columns 9-15, never
     * written as text so it isn't redrawn every frame), temperature in 16-20 */
    temp_text(temp, sizeof(temp), status.cpu_temp);
    snprintf(line, sizeof(line), "CPU %3u%% ", status.cpu);
    write_at(0, 2, line, 9);
    snprintf(line, sizeof(line), " %4s", temp);
    write_at(16, 2, line, 5);
    snprintf(line, sizeof(line), "RAM %3u%% ", status.ram);
    write_at(0, 3, line, 9);
    write_at(16, 3, "", 5);
#ifdef PCSTATUS_GPU
    /* GPU only when the host has a sensor for it */
    if (status.gpu_temp == NA8) {
        write_line(4, "");
    } else {
        temp_text(temp, sizeof(temp), status.gpu_temp);
        snprintf(line, sizeof(line), "GPU              %4s", temp);
        write_line(4, line);
    }
#else
    write_line(4, "");
#endif
    draw_bar(54, 2, 42, status.cpu);
    draw_bar(54, 3, 42, status.ram);

    write_line(5, "");
    if (status.notifications == 0xFFFF) {
        write_line(6, "Notifications     --");
    } else {
        snprintf(line, sizeof(line), "Notifications  %5u", status.notifications);
        write_line(6, line);
    }
    write_line(7, "");
}
#endif
