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
#include "graphics/theme/theme.h"

extern lv_style_t style_screen;
extern lv_style_t style_container;
extern lv_style_t style_text;
extern lv_style_t style_text_secondary;
extern lv_style_t style_arc_main;
extern lv_style_t style_arc_positive;
extern lv_style_t style_arc_knob;
extern lv_style_t style_bar_main;
extern lv_style_t style_bar_positive;
extern lv_style_t style_indice;

extern lv_style_t style_text_time1;
extern lv_style_t style_text_time2;
extern lv_style_t style_text_time3;

extern lv_style_t style_key;
extern lv_style_t style_key_trans;
extern lv_style_t style_key_pressed;

extern lv_style_t style_pomo_breath;
extern lv_style_t style_pomo_bar_bg;
extern lv_style_t style_pomo_bar_indicator;

void init_styles(void);
void apply_theme(void);
