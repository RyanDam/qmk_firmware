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

extern bool layers_running;

void layers_cb(lv_timer_t *timer);

lv_obj_t *screen_layers_init(void);

void screen_layers_set_indice(uint8_t layer_idx);

uint8_t screen_layers_get_current_layer_idx(void);

void screen_layers_set_key_code(uint16_t keycode, keyrecord_t *record);

void screen_layers_stop(void);

void screen_layers_reload(void);
