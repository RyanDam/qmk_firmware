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

LV_FONT_DECLARE(barlow)
LV_FONT_DECLARE(inter)
LV_FONT_DECLARE(bai_jamjuree)
LV_FONT_DECLARE(dm_serif)

lv_style_t style_screen;
lv_style_t style_container;
lv_style_t style_text;
lv_style_t style_arc_main;
lv_style_t style_arc_positive;
lv_style_t style_arc_knob;
lv_style_t style_indice;

lv_style_t style_text_time1;
lv_style_t style_text_time2;
lv_style_t style_text_time3;

lv_style_t style_key;
lv_style_t style_key_trans;
lv_style_t style_key_pressed;

void init_styles(void) {
    lv_style_init(&style_screen);
    lv_style_set_bg_opa(&style_screen, LV_OPA_100);
    lv_style_set_bg_color(&style_screen, lv_color_black());

    lv_style_init(&style_indice);
    lv_style_set_pad_top(&style_indice, 0);
    lv_style_set_pad_bottom(&style_indice, 0);
    lv_style_set_pad_left(&style_indice, 0);
    lv_style_set_pad_right(&style_indice, 0);
    lv_style_set_border_width(&style_indice, 1);
    lv_style_set_width(&style_indice, 9);
    lv_style_set_height(&style_indice, 9);
    lv_style_set_bg_opa(&style_indice, LV_OPA_100);
    lv_style_set_bg_color(&style_indice, lv_color_hex(0xffffff));
    lv_style_set_border_color(&style_indice, lv_color_hex(0xffffff));

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
    lv_style_set_text_font(&style_text, &barlow);

    lv_style_init(&style_key);
    lv_style_set_pad_top(&style_key, 2);
    lv_style_set_pad_bottom(&style_key, 0);
    lv_style_set_pad_left(&style_key, 0);
    lv_style_set_pad_right(&style_key, 0);
    lv_style_set_border_width(&style_key, 1);
    lv_style_set_width(&style_key, 38);
    lv_style_set_height(&style_key, 22);
    lv_style_set_text_align(&style_key, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style_key, lv_color_hex(0xffffff));
    lv_style_set_bg_color(&style_key, lv_color_hex(0x000000));
    lv_style_set_bg_opa(&style_key, LV_OPA_100);
    lv_style_set_border_color(&style_key, lv_color_hex(0x555555));
    lv_style_set_radius(&style_key, 5);
    lv_style_set_text_font(&style_key, &barlow);

    lv_style_init(&style_key_trans);
    lv_style_set_pad_top(&style_key_trans, 2);
    lv_style_set_pad_bottom(&style_key_trans, 0);
    lv_style_set_pad_left(&style_key_trans, 0);
    lv_style_set_pad_right(&style_key_trans, 0);
    lv_style_set_border_width(&style_key_trans, 0);
    lv_style_set_width(&style_key_trans, 38);
    lv_style_set_height(&style_key_trans, 22);
    lv_style_set_text_align(&style_key_trans, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style_key_trans, lv_color_hex(0xffffff));
    lv_style_set_bg_color(&style_key_trans, lv_color_hex(0x000000));
    lv_style_set_bg_opa(&style_key_trans, LV_OPA_100);
    // lv_style_set_border_color(&style_key_trans, lv_color_hex(0x555555));
    // lv_style_set_radius(&style_key_trans, 5);
    lv_style_set_text_font(&style_key_trans, &barlow);

    lv_style_init(&style_key_pressed);
    lv_style_set_pad_top(&style_key_pressed, 2);
    lv_style_set_pad_bottom(&style_key_pressed, 0);
    lv_style_set_pad_left(&style_key_pressed, 0);
    lv_style_set_pad_right(&style_key_pressed, 0);
    lv_style_set_border_width(&style_key_pressed, 1);
    lv_style_set_width(&style_key_pressed, 38);
    lv_style_set_height(&style_key_pressed, 22);
    lv_style_set_text_align(&style_key_pressed, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style_key_pressed, lv_color_hex(0x000000));
    lv_style_set_bg_color(&style_key_pressed, lv_color_hex(0xffffff));
    lv_style_set_bg_opa(&style_key_pressed, LV_OPA_100);
    lv_style_set_border_color(&style_key_pressed, lv_color_hex(0x555555));
    lv_style_set_radius(&style_key_pressed, 5);
    lv_style_set_text_font(&style_key_pressed, &barlow);

    lv_style_init(&style_arc_main);
    lv_style_set_arc_color(&style_arc_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_arc_width(&style_arc_main, 4);

    lv_style_init(&style_arc_positive);
    lv_style_set_arc_color(&style_arc_positive, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_arc_width(&style_arc_positive, 4);

    lv_style_init(&style_arc_knob);
    lv_style_set_arc_color(&style_arc_knob, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_arc_width(&style_arc_knob, 6);

    lv_style_init(&style_text_time1);
    lv_style_set_pad_top(&style_text_time1, 0);
    lv_style_set_pad_bottom(&style_text_time1, 0);
    lv_style_set_pad_left(&style_text_time1, 0);
    lv_style_set_pad_right(&style_text_time1, 0);
    lv_style_set_text_color(&style_text_time1, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_text_time1, &inter);

    lv_style_init(&style_text_time2);
    lv_style_set_pad_top(&style_text_time2, 0);
    lv_style_set_pad_bottom(&style_text_time2, 0);
    lv_style_set_pad_left(&style_text_time2, 0);
    lv_style_set_pad_right(&style_text_time2, 0);
    lv_style_set_text_color(&style_text_time2, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_text_time2, &bai_jamjuree);

    lv_style_init(&style_text_time3);
    lv_style_set_pad_top(&style_text_time3, 0);
    lv_style_set_pad_bottom(&style_text_time3, 0);
    lv_style_set_pad_left(&style_text_time3, 0);
    lv_style_set_pad_right(&style_text_time3, 0);
    lv_style_set_text_color(&style_text_time3, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_text_time3, &dm_serif);

}
