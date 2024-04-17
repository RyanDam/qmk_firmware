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

#include "graphics/screens/screen_time.h"
#include "graphics/screens/styles.h"
#include "graphics/lvgl_helpers.h"
#include "eeprom/cb_eeprom.h"

static lv_obj_t *screen_time = NULL;
static lv_obj_t *time_text;
static lv_obj_t *indicator_text;
static lv_obj_t *date_text;
static lv_obj_t *time_holder;
static lv_obj_t *datetime_holder;

lv_obj_t * screen_time_init(void) {
    screen_time = lv_obj_create(NULL);
    lv_obj_add_style(screen_time, &style_screen, 0);
    use_flex_column(screen_time);

    lv_obj_t *datetime_holder = lv_obj_create(screen_time);
    lv_obj_add_style(datetime_holder, &style_container, 0);
    use_flex_column(datetime_holder);
    lv_obj_set_style_pad_row(datetime_holder, 10, 0);

    date_text  = lv_label_create(datetime_holder);
    lv_label_set_text(date_text, "-- / -- / ---");
    lv_obj_add_style(date_text, &style_text, 0);

    time_holder = lv_obj_create(datetime_holder);
    lv_obj_add_style(time_holder, &style_container, 0);
    use_flex_row(time_holder);

    time_text  = lv_label_create(time_holder);
    lv_label_set_text(time_text, "--:--");
    lv_obj_add_style(time_text, &style_text_time1, 0);

    indicator_text  = lv_label_create(time_holder);
    lv_label_set_text(indicator_text, "--");

    return screen_time;
}

void screen_time_set_time(uint8_t hour, uint8_t minute) {

    switch (config.time_format) {
        case coban_time_format_12h: {
            lv_label_set_text_fmt(time_text, "%02d:%02d", hour%12, minute);
            break;
        }
        case coban_time_format_24h:
        default: {
            lv_label_set_text_fmt(time_text, "%02d:%02d", hour, minute);
            break;
        }
    }

    switch (config.time_indicator) {
        case coban_time_indicator_ampm: {
            if (indicator_text != NULL) {
                if (hour < 12) {
                    lv_label_set_text_fmt(indicator_text, "AM");
                } else {
                    lv_label_set_text_fmt(indicator_text, "PM");
                }
            }
            break;
        }
        case coban_time_indicator_default:
        default:
            // do nothing
            break;
    }
}

void screen_time_set_date(uint8_t day, uint8_t month, uint8_t year) {
    switch (config.date_format) {
        case coban_date_format_mmddyyyy: {
            lv_label_set_text_fmt(date_text, "%02d / %02d / 2%03d", month, day, year);
            break;
        }
        case coban_date_format_yyyymmdd: {
            lv_label_set_text_fmt(date_text, "2%03d / %02d / %02d", year, month, day);
            break;
        }
        case coban_date_format_ddmmyyyy:
        default:{
            lv_label_set_text_fmt(date_text, "%02d / %02d / 2%03d", day, month, year);
            break;
        }
    }
}

void screen_time_set_datetime(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year) {
    screen_time_set_date(day, month, year);
    screen_time_set_time(hour, minute);
}

void screen_time_set_format(uint8_t time_style, uint8_t time_format, uint8_t time_indicator, uint8_t date_format, uint8_t date_visibility) {
    screen_time_set_time_style(time_style);
    screen_time_set_time_format(time_format);
    screen_time_set_time_indicator(time_indicator);

    screen_time_set_date_format(date_format);
    screen_time_set_date_visibility(date_visibility);
}

void screen_time_set_time_style(uint8_t time_style) {
    config.time_style_id = time_style;

    switch (config.time_style_id) {
        case coban_time_style_2: {
            lv_obj_add_style(time_text, &style_text_time2, 0);
            break;
        }
        case coban_time_style_3: {
            lv_obj_add_style(time_text, &style_text_time3, 0);
            break;
        }
        case coban_time_style_4: {
            lv_obj_add_style(time_text, &style_text_time4, 0);
            break;
        }
        case coban_time_style_5: {
            lv_obj_add_style(time_text, &style_text_time5, 0);
            break;
        }
        case coban_time_style_1:
        default: {
            lv_obj_add_style(time_text, &style_text_time1, 0);
            break;
        }
    }
}

void screen_time_set_time_format(uint8_t time_format) {
    config.time_format = time_format;
}

void screen_time_set_time_indicator(uint8_t time_indicator) {
    config.time_indicator = time_indicator;
    switch (config.time_indicator) {
        case coban_time_indicator_ampm: {
            if (indicator_text == NULL) {
                indicator_text  = lv_label_create(time_holder);
                lv_label_set_text(indicator_text, "--");
            }
            break;
        }
        case coban_time_indicator_default:
        default: {
            if (indicator_text != NULL) {
                lv_obj_del(indicator_text);
                indicator_text = NULL;
            }
            break;
        }
    }
}

void screen_time_set_date_format(uint8_t date_format) {
    config.date_format = date_format;
}

void screen_time_set_date_visibility(uint8_t date_visibility) {
    config.date_visibility = date_visibility;

    switch (config.date_visibility) {
        case coban_date_visibility_visible: {
            if (date_text == NULL) {
                date_text = lv_label_create(datetime_holder);
                lv_label_set_text(date_text, "-- / -- / ---");
                lv_obj_add_style(date_text, &style_text, 0);
            }
            break;
        }
        case coban_date_visibility_invisible:
        default: {
            if (date_text != NULL) {
                lv_obj_del(date_text);
                date_text = NULL;
            }
            break;
        }
    }
}
