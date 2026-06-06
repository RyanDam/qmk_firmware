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
#include <stdint.h>

typedef struct _cb_config_t {
    uint8_t screen_idx;                  // default screen
    uint8_t screen_brightness;           //
    uint8_t screen_switch_layer;         // 1, switch to layer when key event
    uint8_t screen_switch_layer_timeout; // in sec, switch back to original screen if not have key event
    uint8_t screen_idle_timeout;         // in minute, turn off screen if not have key event
    uint8_t screen_background_enable;    // 0=black bg, 1=gif bg

    uint8_t layer_switch_default;
    uint8_t layer_switch_default_timeout;

    // datetime format
    uint8_t time_style_id;
    uint8_t time_format;
    uint8_t time_indicator;
    uint8_t date_format;
    uint8_t date_visibility;

    // pomodoro
    uint8_t pomo_num_set;
    uint8_t pomo_work_duration;
    uint8_t pomo_rest_duration;
    uint8_t pomo_noti_mode;

    // audio volume
    uint8_t audio_volume;

    // gif data
    uint32_t gif_data_size;

    // macro names (16 macros, 6 chars each)
    uint8_t macro_names[16][6];

    // stats screen config
    uint8_t stats_layout_id;
    uint8_t stats_data_ids[4];

    // theme colors: 5 RGB colors (15 bytes total)
    // [0..2]  background
    // [3..5]  text_primary
    // [6..8]  text_secondary
    // [9..11] accent
    // [12..14] inactive
    uint8_t theme_colors[15];

    // padding to reach 144 bytes
    uint8_t reserved[3];
} cb_config;

extern cb_config config;

void coban_init_config(void);
void coban_save_config(void);
void coban_load_config(void);
