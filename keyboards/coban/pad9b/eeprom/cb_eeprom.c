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

// Check if the size of the reserved persistent memory is the same as the size of struct apc_config
_Static_assert(sizeof(cb_config) == VIA_EEPROM_CUSTOM_CONFIG_SIZE, "Mismatch in keyboard EECONFIG stored data");

cb_config config;

void coban_init_config(void) {
    config.screen_idx = coban_screen_clock;
    config.screen_brightness = 127;
    config.screen_timeout = 0; // no timeout
    config.screen_timeout_brightness = 55;

    config.time_style_id = coban_time_style_1;
    config.time_format = coban_time_format_24h;
    config.time_indicator = coban_time_indicator_default;
    config.date_format = coban_date_format_ddmmyyyy;
    config.date_visibility = coban_date_visibility_visible;
}

void coban_save_config(void) {
    // Write default value to EEPROM now
    eeprom_write_block(&config, (void *)EEPROM_CB_CONFIG_ADDR, VIA_EEPROM_CUSTOM_CONFIG_SIZE);
}

void coban_load_config(void) {
    // read config from eeprom
    // config.screen_idx = eeprom_read_byte((void*)EEPROM_CB_CONFIG_ADDR);
    eeprom_read_block(&config, (void *)EEPROM_CB_CONFIG_ADDR, VIA_EEPROM_CUSTOM_CONFIG_SIZE);
}
