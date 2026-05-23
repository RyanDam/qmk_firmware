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

#include <stdio.h>
#include "qp_lvgl.h"

#include "graphics/screens/styles.h"
#include "graphics/screens/screen_boot.h"
#include "graphics/screens/screen_time.h"
#include "graphics/screens/screen_hardware_stats.h"
#include "graphics/screens/screen_animation.h"
#include "graphics/screens/screen_layers.h"
// #include "graphics/screens/screen_render.h"
#include "graphics/screens/screen_pomodoro.h"

enum coban_screen_id {
    coban_screen_clock = 0x00,
    coban_screen_stats = 0x01,
    coban_screen_anime = 0x02,
    coban_screen_layer = 0x03,
    // coban_screen_render = 0x04,
    coban_screen_pomodoro = 0x05,

    coban_screen_undefined = 0x99
};

void ui_init(void);

void screen_ui_stop(void);

void screen_ui_reload(void);

enum coban_screen_id current_screen(void);

enum coban_screen_id change_screen(uint8_t screen_idx);

enum coban_screen_id next_screen(bool need_change_screen);

enum coban_screen_id prev_screen(bool need_change_screen);
