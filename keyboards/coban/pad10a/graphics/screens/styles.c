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
LV_FONT_DECLARE(icons)

lv_style_t style_screen;
lv_style_t style_container;
lv_style_t style_text;
lv_style_t style_text_secondary;
lv_style_t style_arc_main;
lv_style_t style_arc_positive;
lv_style_t style_arc_knob;
lv_style_t style_bar_main;
lv_style_t style_bar_positive;
lv_style_t style_indice;

lv_style_t style_text_time1;
lv_style_t style_text_time2;
lv_style_t style_text_time3;

lv_style_t style_key;
lv_style_t style_key_trans;
lv_style_t style_key_pressed;

lv_style_t style_pomo_breath;
lv_style_t style_pomo_bar_bg;
lv_style_t style_pomo_bar_indicator;

void apply_theme(void) {
    const theme_color_t *t = get_theme_colors();

    lv_style_reset(&style_screen);
    lv_style_set_bg_opa(&style_screen, LV_OPA_100);
    lv_style_set_bg_color(&style_screen, t->background);

    lv_style_reset(&style_indice);
    lv_style_set_pad_top(&style_indice, 0);
    lv_style_set_pad_bottom(&style_indice, 0);
    lv_style_set_pad_left(&style_indice, 0);
    lv_style_set_pad_right(&style_indice, 0);
    lv_style_set_border_width(&style_indice, 0);
    lv_style_set_width(&style_indice, 9);
    lv_style_set_height(&style_indice, 9);
    lv_style_set_bg_opa(&style_indice, LV_OPA_100);
    lv_style_set_bg_color(&style_indice, t->accent);

    lv_style_reset(&style_container);
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

    lv_style_reset(&style_text);
    lv_style_set_pad_top(&style_text, 0);
    lv_style_set_pad_bottom(&style_text, 0);
    lv_style_set_pad_left(&style_text, 0);
    lv_style_set_pad_right(&style_text, 0);
    lv_style_set_text_color(&style_text, t->text_primary);
    lv_style_set_text_font(&style_text, &barlow);

    lv_style_reset(&style_text_secondary);
    lv_style_set_pad_top(&style_text_secondary, 0);
    lv_style_set_pad_bottom(&style_text_secondary, 0);
    lv_style_set_pad_left(&style_text_secondary, 0);
    lv_style_set_pad_right(&style_text_secondary, 0);
    lv_style_set_text_color(&style_text_secondary, t->text_secondary);
    lv_style_set_text_font(&style_text_secondary, &barlow);

    lv_style_reset(&style_key);
    lv_style_set_pad_top(&style_key, 3);
    lv_style_set_pad_bottom(&style_key, 0);
    lv_style_set_pad_left(&style_key, 0);
    lv_style_set_pad_right(&style_key, 0);
    lv_style_set_border_width(&style_key, 0);
    lv_style_set_width(&style_key, 38);
    lv_style_set_height(&style_key, 24);
    lv_style_set_text_align(&style_key, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style_key, t->text_primary);
    lv_style_set_bg_color(&style_key, t->inactive);
    lv_style_set_bg_opa(&style_key, LV_OPA_20);
    lv_style_set_radius(&style_key, 5);
    lv_style_set_text_font(&style_key, &barlow);

    lv_style_reset(&style_key_trans);
    lv_style_set_pad_top(&style_key_trans, 3);
    lv_style_set_pad_bottom(&style_key_trans, 0);
    lv_style_set_pad_left(&style_key_trans, 0);
    lv_style_set_pad_right(&style_key_trans, 0);
    lv_style_set_border_width(&style_key_trans, 0);
    lv_style_set_width(&style_key_trans, 38);
    lv_style_set_height(&style_key_trans, 24);
    lv_style_set_text_align(&style_key_trans, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style_key_trans, t->text_primary);
    lv_style_set_bg_color(&style_key_trans, t->background);
    lv_style_set_bg_opa(&style_key_trans, LV_OPA_TRANSP);
    lv_style_set_text_font(&style_key_trans, &barlow);

    lv_style_reset(&style_key_pressed);
    lv_style_set_pad_top(&style_key_pressed, 3);
    lv_style_set_pad_bottom(&style_key_pressed, 0);
    lv_style_set_pad_left(&style_key_pressed, 0);
    lv_style_set_pad_right(&style_key_pressed, 0);
    lv_style_set_border_width(&style_key_pressed, 0);
    lv_style_set_width(&style_key_pressed, 38);
    lv_style_set_height(&style_key_pressed, 24);
    lv_style_set_text_align(&style_key_pressed, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style_key_pressed, t->text_primary);
    lv_style_set_bg_color(&style_key_pressed, t->inactive);
    lv_style_set_bg_opa(&style_key_pressed, LV_OPA_100);
    lv_style_set_radius(&style_key_pressed, 5);
    lv_style_set_text_font(&style_key_pressed, &barlow);

    lv_style_reset(&style_arc_main);
    lv_style_set_arc_color(&style_arc_main, t->inactive);
    lv_style_set_arc_opa(&style_arc_main, LV_OPA_20);
    lv_style_set_arc_width(&style_arc_main, 8);

    lv_style_reset(&style_arc_positive);
    lv_style_set_arc_color(&style_arc_positive, t->accent);
    lv_style_set_arc_width(&style_arc_positive, 8);

    lv_style_reset(&style_arc_knob);
    lv_style_set_arc_color(&style_arc_knob, t->accent);
    lv_style_set_arc_width(&style_arc_knob, 6);

    lv_style_reset(&style_bar_main);
    lv_style_set_bg_color(&style_bar_main, t->inactive);
    lv_style_set_bg_opa(&style_bar_main, LV_OPA_20);
    lv_style_set_radius(&style_bar_main, 3);

    lv_style_reset(&style_bar_positive);
    lv_style_set_bg_color(&style_bar_positive, t->accent);
    lv_style_set_bg_opa(&style_bar_positive, LV_OPA_100);
    lv_style_set_radius(&style_bar_positive, 3);

    lv_style_reset(&style_text_time1);
    lv_style_set_pad_top(&style_text_time1, 0);
    lv_style_set_pad_bottom(&style_text_time1, 0);
    lv_style_set_pad_left(&style_text_time1, 0);
    lv_style_set_pad_right(&style_text_time1, 0);
    lv_style_set_text_color(&style_text_time1, t->text_primary);
    lv_style_set_text_font(&style_text_time1, &inter);

    lv_style_reset(&style_text_time2);
    lv_style_set_pad_top(&style_text_time2, 0);
    lv_style_set_pad_bottom(&style_text_time2, 0);
    lv_style_set_pad_left(&style_text_time2, 0);
    lv_style_set_pad_right(&style_text_time2, 0);
    lv_style_set_text_color(&style_text_time2, t->text_primary);
    lv_style_set_text_font(&style_text_time2, &bai_jamjuree);

    lv_style_reset(&style_text_time3);
    lv_style_set_pad_top(&style_text_time3, 0);
    lv_style_set_pad_bottom(&style_text_time3, 0);
    lv_style_set_pad_left(&style_text_time3, 0);
    lv_style_set_pad_right(&style_text_time3, 0);
    lv_style_set_text_color(&style_text_time3, t->text_primary);
    lv_style_set_text_font(&style_text_time3, &dm_serif);

    lv_style_reset(&style_pomo_breath);
    lv_style_set_bg_color(&style_pomo_breath, t->accent);
    lv_style_set_bg_opa(&style_pomo_breath, LV_OPA_20);

    lv_style_reset(&style_pomo_bar_bg);
    lv_style_set_bg_color(&style_pomo_bar_bg, t->inactive);
    lv_style_set_bg_opa(&style_pomo_bar_bg, LV_OPA_30);

    lv_style_reset(&style_pomo_bar_indicator);
    lv_style_set_bg_color(&style_pomo_bar_indicator, t->accent);
    lv_style_set_bg_opa(&style_pomo_bar_indicator, LV_OPA_100);
}

void init_styles(void) {
    lv_style_init(&style_screen);
    lv_style_init(&style_indice);
    lv_style_init(&style_container);
    lv_style_init(&style_text);
    lv_style_init(&style_text_secondary);
    lv_style_init(&style_key);
    lv_style_init(&style_key_trans);
    lv_style_init(&style_key_pressed);
    lv_style_init(&style_arc_main);
    lv_style_init(&style_arc_positive);
    lv_style_init(&style_arc_knob);
    lv_style_init(&style_bar_main);
    lv_style_init(&style_bar_positive);
    lv_style_init(&style_text_time1);
    lv_style_init(&style_text_time2);
    lv_style_init(&style_text_time3);
    lv_style_init(&style_pomo_breath);
    lv_style_init(&style_pomo_bar_bg);
    lv_style_init(&style_pomo_bar_indicator);

    apply_theme();
}