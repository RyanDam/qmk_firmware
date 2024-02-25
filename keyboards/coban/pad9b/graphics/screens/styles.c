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

#include "graphics/screens/styles.h"

lv_style_t style_screen;
lv_style_t style_container;
lv_style_t style_text;
lv_style_t style_arc_main;
lv_style_t style_arc_positive;
lv_style_t style_arc_knob;

void init_styles(void) {
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, lv_color_black());

    lv_style_init(&style_container);
    lv_style_set_pad_top(&style_container, 0);
    lv_style_set_pad_bottom(&style_container, 0);
    lv_style_set_pad_left(&style_container, 0);
    lv_style_set_pad_right(&style_container, 0);
    lv_style_set_bg_opa(&style_container, 0);
    lv_style_set_border_width(&style_container, 0);
    lv_style_set_width(&style_container, LV_SIZE_CONTENT);
    lv_style_set_height(&style_container, LV_SIZE_CONTENT);
    lv_style_set_pad_row(&style_container, 0);
    lv_style_set_pad_column(&style_container, 12);

    lv_style_init(&style_text);
    lv_style_set_pad_top(&style_text, 0);
    lv_style_set_pad_bottom(&style_text, 0);
    lv_style_set_pad_left(&style_text, 0);
    lv_style_set_pad_right(&style_text, 0);
    lv_style_set_text_color(&style_text, lv_color_hex(0xffffff));

    lv_style_init(&style_arc_main);
    lv_style_set_arc_color(&style_arc_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_arc_width(&style_arc_main, 4);

    lv_style_init(&style_arc_positive);
    lv_style_set_arc_color(&style_arc_positive, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_arc_width(&style_arc_positive, 4);

    lv_style_init(&style_arc_knob);
    lv_style_set_arc_color(&style_arc_knob, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_arc_width(&style_arc_knob, 6);
}
