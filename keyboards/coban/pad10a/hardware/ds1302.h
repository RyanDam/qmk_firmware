/* Copyright 2024 RyanDam (https://github.com/RyanDam)
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

#include "quantum.h"

#ifdef DS1302_ENABLE

// DS1302 Register addresses
// datasheet: https://www.lcsc.com/datasheet/C42411640.pdf
// reference: https://github.com/odeevee/DS1302_CircuitPython/blob/master/ds1302.py
#define DS1302_REG_SECOND   0x80
#define DS1302_REG_MINUTE   0x82
#define DS1302_REG_HOUR     0x84
#define DS1302_REG_DAY      0x86
#define DS1302_REG_MONTH    0x88
#define DS1302_REG_WEEKDAY  0x8A
#define DS1302_REG_YEAR     0x8C
#define DS1302_REG_WP       0x8E
#define DS1302_REG_CTRL     0x90

#define DS1302_READ_BIT     0x01
#define DS1302_WRITE_BIT    0x00
#define DS1302_MULTI_BIT    0x80

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t weekday;
    uint8_t day;
    uint8_t month;
    uint8_t year; // year is 0->99 (minus 2000)
} ds1302_datetime_t;

extern bool ds1302_initialized;

bool ds1302_init(void);
bool ds1302_read_datetime(ds1302_datetime_t *datetime);
bool ds1302_write_datetime(const ds1302_datetime_t *datetime);

#endif // DS1302_ENABLE
