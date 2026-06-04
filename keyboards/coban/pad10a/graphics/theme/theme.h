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

#include "qp_lvgl.h"

enum coban_theme_id { coban_theme_dark = 0, coban_theme_light = 1, coban_theme_gray = 2, coban_theme_pink = 3, coban_theme_olive = 4, coban_theme_orange = 5, coban_theme_count };

typedef struct {
    lv_color_t bg;                   // main background (maybe transparent when gif is used as background)
    lv_color_t text_primary;         // main text: clock, pomo
    lv_color_t text_secondary;       // not used yet
    lv_color_t accent;               // stat arc fg, bar fg
    lv_color_t surface;              // layer key text + bg, pomo breath
    lv_color_t surface_pressed;      // layer bg pressed
    lv_color_t surface_pressed_text; // layer key pressed
    lv_color_t inactive;             // layer indice bg, stats arc bg
    lv_color_t pomo_bg;              // pomo bar bg
    lv_color_t pomo_indicator;       // pomo bar fg
} theme_color_t;

const theme_color_t *get_current_theme(void);
void                 apply_theme(void);
