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

static lv_obj_t *screen_time;
static lv_obj_t *time_text;
static lv_obj_t *date_text;

lv_obj_t * screen_time_init(void) {
    screen_time = lv_obj_create(NULL);
    lv_obj_add_style(screen_time, &style_screen, 0);
    use_flex_column(screen_time);

    lv_obj_t *mods = lv_obj_create(screen_time);
    lv_obj_add_style(mods, &style_container, 0);
    use_flex_column(mods);

    date_text  = lv_label_create(mods);
    lv_label_set_text(date_text, "18/02/2024");
    lv_obj_add_style(date_text, &style_text, 0);

    time_text  = lv_label_create(mods);
    lv_label_set_text(time_text, "20:45");
    lv_obj_add_style(time_text, &style_text, 0);
    resize_font(time_text, 36);

    lv_scr_load(screen_time);
    return screen_time;
}

bool screen_time_is_visible(void) {
    return true;
}

void screen_time_set_visible(void) {

}

void screen_time_set_invisible(void) {

}

void screen_time_set_time(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year) {
    lv_label_set_text_fmt(date_text, "%02d/%02d/2%03d", day, month, year);
    lv_label_set_text_fmt(time_text, "%02d:%02d", hour, minute);
}
