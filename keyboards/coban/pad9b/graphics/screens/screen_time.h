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

#include "qp_lvgl.h"

enum coban_time_style_id {
    coban_time_style_1 = 0x00,
    coban_time_style_2 = 0x01,
    coban_time_style_3 = 0x02,
    coban_time_style_4 = 0x03,
    coban_time_style_5 = 0x04,
};

enum coban_time_format_id {
    coban_time_format_12h = 0x00,
    coban_time_format_24h = 0x01
};

enum coban_time_indicator_id {
    coban_time_indicator_default = 0x00,
    coban_time_indicator_ampm = 0x01,
};

enum coban_date_format_id {
    coban_date_format_ddmmyyyy = 0x00,
    coban_date_format_mmddyyyy = 0x01,
    coban_date_format_yyyymmdd = 0x02,
};

enum coban_date_visibility_id {
    coban_date_visibility_visible = 0x00,
    coban_date_visibility_invisible = 0x01
};

lv_obj_t * screen_time_init(void);

void screen_time_set_time(uint8_t hour, uint8_t minute);

void screen_time_set_date(uint8_t day, uint8_t month, uint8_t year);

void screen_time_set_datetime(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year);

void screen_time_set_format(uint8_t time_style, uint8_t time_format, uint8_t time_indicator, uint8_t date_format, uint8_t date_visibility);

void screen_time_set_time_style(uint8_t time_style);

void screen_time_set_time_format(uint8_t time_format);

void screen_time_set_time_indicator(uint8_t time_indicator);

void screen_time_set_date_format(uint8_t date_format);

void screen_time_set_date_visibility(uint8_t date_visibility);
