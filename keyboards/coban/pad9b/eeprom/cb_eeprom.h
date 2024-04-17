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

typedef struct _cb_config_t {
    uint8_t screen_idx;
    uint8_t screen_brightness;
    uint8_t screen_timeout;
    uint8_t screen_timeout_brightness;

    // datetime format
    uint8_t time_style_id;
    uint8_t time_format;
    uint8_t time_indicator;
    uint8_t date_format;
    uint8_t date_visibility;
} cb_config;

extern cb_config config;

void coban_init_config(void);
void coban_save_config(void);
void coban_load_config(void);
