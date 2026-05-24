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

#include "eeprom/cb_eeprom.h"
#include "graphics/ui.h"
// #include "eeprom.h"
#include "via.h"

// Check if the size of the reserved persistent memory is the same as the size of struct apc_config
_Static_assert(sizeof(cb_config) == EEPROM_CB_CONFIG_SIZE, "Mismatch in keyboard EECONFIG stored data");

cb_config config;

void coban_init_config(void) {
    config.screen_idx                  = coban_screen_clock;
    config.screen_brightness           = 127;
    config.screen_switch_layer         = 1;
    config.screen_switch_layer_timeout = 5;
    config.screen_idle_timeout         = 15;

    config.layer_switch_default         = 1;
    config.layer_switch_default_timeout = 60; // in sec, switch back to default layer if not have key event

    config.time_style_id   = coban_time_style_1;
    config.time_format     = coban_time_format_24h;
    config.time_indicator  = coban_time_indicator_default;
    config.date_format     = coban_date_format_ddmmyyyy;
    config.date_visibility = coban_date_visibility_visible;

    config.pomo_num_set       = 4;
    config.pomo_work_duration = 25;
    config.pomo_rest_duration = 5;
    config.pomo_noti_mode     = coban_pomo_noti_mode_beep;

    config.audio_volume = 0;

    config.gif_data_size = EEPROM_MAX_GIF_SIZE;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 6; j++) {
            config.macro_names[i][j] = 0;
        }
    }

 #ifdef COBAN_STATS_SCREEN_ENABLE
    config.stats_layout_id   = coban_stats_layout_2x1;
    config.stats_data_ids[0] = coban_stats_data_cpu_util;
    config.stats_data_ids[1] = coban_stats_data_ram;
    config.stats_data_ids[2] = coban_stats_data_cpu_temp;
    config.stats_data_ids[3] = coban_stats_data_cpu_util;
#endif
}

void coban_save_config(void) {
    // Write default value to EEPROM now
    via_update_custom_config(&config, 0, EEPROM_CB_CONFIG_SIZE);
}

void coban_load_config(void) {
    // read config from eeprom
    // config.screen_idx = eeprom_read_byte((void*)EEPROM_CB_CONFIG_ADDR);
    via_read_custom_config(&config, 0, EEPROM_CB_CONFIG_SIZE);
}
