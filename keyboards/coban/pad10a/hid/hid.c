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

#include "hid/hid.h"
#include "raw_hid.h"

#include "graphics/ui.h"
#include "eeprom/cb_eeprom.h"
#include "utils/audio_volume.h"
#include "hardware/flash.h"
#include <hardware/sync.h>
#ifdef DS1302_ENABLE
#    include "hardware/ds1302.h"
#endif // DS1302_ENABLE
#define GIF_TRANSFER_BLOCK_SIZE 25

void cb_raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &(data[0]);
    uint8_t *command_data = &(data[1]); // data length: 29

    switch (*command_id) {
        case coban_cmd_id_set_screen: {
            config.screen_idx                  = command_data[0];
            config.screen_switch_layer         = command_data[1];
            config.screen_switch_layer_timeout = command_data[2];
            config.screen_idle_timeout         = command_data[3];
            change_screen(config.screen_idx);
            break;
        }
        case coban_cmd_id_set_time: {
            uint8_t hour   = command_data[0];
            uint8_t minute = command_data[1];
            uint8_t second = command_data[2];
            uint8_t day    = command_data[3];
            uint8_t month  = command_data[4];
            uint8_t year   = command_data[5];
            screen_time_sync_datetime(hour, minute, second, day, month, year);
#ifdef DS1302_ENABLE
            // sync datetime to ds1302 chip
            ds1302_datetime_t rtc_time;
            rtc_time.second  = second;
            rtc_time.minute  = minute;
            rtc_time.hour    = hour;
            rtc_time.weekday = 0;
            rtc_time.day     = day;
            rtc_time.month   = month;
            rtc_time.year    = year;
            ds1302_write_datetime(&rtc_time);
#endif
            break;
        }
        case coban_cmd_id_set_stats_config: {
            config.stats_layout_id = command_data[0];
            for (int i = 0; i < 4; i++) {
                config.stats_data_ids[i] = command_data[1 + i];
            }
            screen_hardware_stat_set_layout(config.stats_layout_id);
            coban_save_config();
            break;
        }
        case coban_cmd_id_set_stats_data: {
            for (int i = 0; i < 4; i++) {
                uint8_t data_id = command_data[i * 6 + 0];
                if (data_id == coban_stats_data_none) {
                    continue;
                }
                uint16_t max_value = command_data[i * 6 + 1] | (command_data[i * 6 + 2] << 8);
                uint8_t  unit_id   = command_data[i * 6 + 3];
                uint16_t value     = command_data[i * 6 + 4] | (command_data[i * 6 + 5] << 8);
                screen_hardware_stat_set_data(data_id, value, max_value, unit_id);
            }
            break;
        }
        case coban_cmd_id_set_time_format: {
            uint8_t time_style      = command_data[0];
            uint8_t time_format     = command_data[1];
            uint8_t time_indicator  = command_data[2];
            uint8_t date_format     = command_data[3];
            uint8_t date_visibility = command_data[4];
            screen_time_set_format(time_style, time_format, time_indicator, date_format, date_visibility);
            screen_pomodoro_set_time_style(time_style);
            break;
        }
        case coban_cmd_id_set_layer: {
            config.layer_switch_default         = command_data[0];
            config.layer_switch_default_timeout = command_data[1];
            break;
        }
        case coban_cmd_id_set_gif_buffer: {
            // first, stop animation to prevent crash because of data writing
            screen_animation_stop();

            uint8_t  offset_1 = command_data[0];
            uint8_t  offset_2 = command_data[1];
            uint8_t  offset_3 = command_data[2];
            uint8_t  offset_4 = command_data[3];
            uint32_t offset   = 0x00000000 | (offset_1 << 24) | (offset_2 << 16) | (offset_3 << 8) | (offset_4 << 0);
            if ((offset + GIF_TRANSFER_BLOCK_SIZE) <= EEPROM_MAX_GIF_SIZE) {
                uint8_t *data = &(command_data[4]);
                for (uint32_t i = 0; i < GIF_TRANSFER_BLOCK_SIZE; i++) {
                    *(gif_data + offset + i) = *(data + i);
                }
            }
            break;
        }
        case coban_cmd_id_set_gif_size: {
            uint8_t  offset_1     = command_data[0];
            uint8_t  offset_2     = command_data[1];
            uint8_t  offset_3     = command_data[2];
            uint8_t  offset_4     = command_data[3];
            uint32_t gif_datasize = 0x00000000 | (offset_1 << 24) | (offset_2 << 16) | (offset_3 << 8) | (offset_4 << 0);

            config.gif_data_size      = gif_datasize;
            gif_data_header.data_size = config.gif_data_size;
            if (current_screen() == coban_screen_anime) {
                screen_animation_reload();
            }
            break;
        }
        case coban_cmd_id_set_pomo_config: {
            config.pomo_num_set       = command_data[0];
            config.pomo_work_duration = command_data[1];
            config.pomo_rest_duration = command_data[2];
            config.pomo_noti_mode     = command_data[3];
            screen_pomodoro_ui_update();
            break;
        }
        case coban_cmd_id_set_audio_volume: {
            uint8_t volume = command_data[0];
            if (volume > 4) {
                volume = 4;
            }
            audio_volume_set(volume);
            config.audio_volume = volume;
            coban_save_config();
            break;
        }
        case coban_cmd_id_set_macro_name: {
            uint8_t macro_idx = command_data[0];
            if (macro_idx < 16) {
                for (int i = 0; i < 6; i++) {
                    config.macro_names[macro_idx][i] = command_data[i + 1];
                }
                coban_save_config();
            }
            break;
        }
        case coban_cmd_id_reboot_board: {
            soft_reset_keyboard();
            break;
        }
        case coban_cmd_id_reset_board: {
            reset_keyboard();
            break;
        }
        case coban_cmd_id_reset_layer_ui: {
            uint8_t target_layer_idx  = command_data[0];
            uint8_t current_layer_idx = screen_layers_get_current_layer_idx();
            if (target_layer_idx == current_layer_idx) {
                screen_layers_set_indice(target_layer_idx);
            }
        }
        default:
            break;
    }
}

void cb_raw_hid_response_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &(data[0]);
    uint8_t *command_data = &(data[1]);

    switch (*command_id) {
        case coban_cmd_id_set_screen: {
            *(command_data + 0) = 0xff & config.screen_idx;
            *(command_data + 1) = 0xff & config.screen_switch_layer;
            *(command_data + 2) = 0xff & config.screen_switch_layer_timeout;
            *(command_data + 3) = 0xff & config.screen_idle_timeout;
            break;
        }
        case coban_cmd_id_set_time_format: {
            *(command_data + 0) = 0xff & config.time_style_id;
            *(command_data + 1) = 0xff & config.time_format;
            *(command_data + 2) = 0xff & config.time_indicator;
            *(command_data + 3) = 0xff & config.date_format;
            *(command_data + 4) = 0xff & config.date_visibility;
            break;
        }
        case coban_cmd_id_set_layer: {
            *(command_data + 0) = 0xff & config.layer_switch_default;
            *(command_data + 1) = 0xff & config.layer_switch_default_timeout;
            break;
        }
        case coban_cmd_id_set_stats_config: {
            *(command_data + 0) = 0xff & config.stats_layout_id;
            for (int i = 0; i < 4; i++) {
                *(command_data + 1 + i) = 0xff & config.stats_data_ids[i];
            }
            break;
        }
        case coban_cmd_id_set_gif_buffer: {
            uint8_t  offset_1 = command_data[0];
            uint8_t  offset_2 = command_data[1];
            uint8_t  offset_3 = command_data[2];
            uint8_t  offset_4 = command_data[3];
            uint32_t offset   = 0x00000000 | (offset_1 << 24) | (offset_2 << 16) | (offset_3 << 8) | (offset_4 << 0);

            if ((offset + GIF_TRANSFER_BLOCK_SIZE) <= EEPROM_MAX_GIF_SIZE) {
                for (uint32_t i = 0; i < GIF_TRANSFER_BLOCK_SIZE; i++) {
                    *(command_data + i + 4) = *(gif_data + offset + i);
                }
            }
            break;
        }
        case coban_cmd_id_set_gif_size: {
            *(command_data + 0) = 0xff & ((config.gif_data_size & 0xff000000) >> 24);
            *(command_data + 1) = 0xff & ((config.gif_data_size & 0x00ff0000) >> 16);
            *(command_data + 2) = 0xff & ((config.gif_data_size & 0x0000ff00) >> 8);
            *(command_data + 3) = 0xff & ((config.gif_data_size & 0x000000ff) >> 0);
            break;
        }
        case coban_cmd_id_set_gif_flash: {
            uint8_t  offset_1 = command_data[0];
            uint8_t  offset_2 = command_data[1];
            uint8_t  offset_3 = command_data[2];
            uint8_t  offset_4 = command_data[3];
            uint32_t offset   = 0x00000000 | (offset_1 << 24) | (offset_2 << 16) | (offset_3 << 8) | (offset_4 << 0);

            if ((offset + GIF_TRANSFER_BLOCK_SIZE) <= EEPROM_MAX_GIF_SIZE) {
                const uint8_t *pointer = (const uint8_t *)(XIP_BASE + EEROM_CB_GIF_ADDR + offset);
                for (int i = 0; i < GIF_TRANSFER_BLOCK_SIZE; i++) {
                    *(command_data + i + 4) = *(pointer + i);
                }
            }
            break;
        }
        case coban_cmd_id_set_pomo_config: {
            *(command_data + 0) = 0xff & config.pomo_num_set;
            *(command_data + 1) = 0xff & config.pomo_work_duration;
            *(command_data + 2) = 0xff & config.pomo_rest_duration;
            *(command_data + 3) = 0xff & config.pomo_noti_mode;
            break;
        }
        case coban_cmd_id_set_audio_volume: {
            *(command_data + 0) = 0xff & config.audio_volume;
            break;
        }
        case coban_cmd_id_set_macro_name: {
            uint8_t macro_idx = command_data[0];
            if (macro_idx < 16) {
                for (int i = 0; i < 6; i++) {
                    *(command_data + i + 1) = config.macro_names[macro_idx][i];
                }
            }
            break;
        }
        default:
            break;
    }
}

void cb_config_save(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
    // uint8_t *command_data = &(data[1]);

    switch (*command_id) {
        case coban_cmd_id_save_eeprom: {
            coban_save_config();
            break;
        }
        case coban_cmd_id_save_gif_data: {
            // save gif to flash
            uint32_t ints = save_and_disable_interrupts();
            // Calculate the absolute address in flash memory
            if (EEROM_CB_GIF_ADDR + EEPROM_MAX_GIF_SIZE <= PICO_FLASH_SIZE_BYTES) {
                // Write data to flash
                int    plus_erase     = EEPROM_MAX_GIF_SIZE % FLASH_SECTOR_SIZE > 0 ? 1 : 0;
                size_t datasize_erase = (((size_t)EEPROM_MAX_GIF_SIZE / FLASH_SECTOR_SIZE) + plus_erase) * FLASH_SECTOR_SIZE;
                // first, earse target flash is required befor write data to it
                flash_range_erase(EEROM_CB_GIF_ADDR, datasize_erase);

                // Write data to flash
                int    plus_program     = EEPROM_MAX_GIF_SIZE % FLASH_PAGE_SIZE > 0 ? 1 : 0;
                size_t datasize_program = (((size_t)EEPROM_MAX_GIF_SIZE / FLASH_PAGE_SIZE) + plus_program) * FLASH_PAGE_SIZE;
                // program flash
                flash_range_program(EEROM_CB_GIF_ADDR, gif_data, datasize_program);
            } else {
                // Handle error: offset exceeds flash size
            }
            restore_interrupts(ints);
            break;
        }
        default:
            break;
    }
}

#ifdef RAW_ENABLE

void eeconfig_init_user(void) {
    coban_init_config();
    coban_save_config();
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data ]
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
        switch (*command_id) {
            case id_custom_set_value: {
                // cb_config_set_value(value_id_and_data);
                cb_raw_hid_receive_kb(value_id_and_data, length - 2);
                break;
            }
            case id_custom_get_value: {
                cb_raw_hid_response_kb(value_id_and_data, length - 2);
                break;
            }
            case id_custom_save: {
                cb_config_save(value_id_and_data, length - 2);
                break;
            }
            default: {
                // Unhandled message.
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }
    // Return the unhandled state
    *command_id = id_unhandled;

    // DO NOT call raw_hid_send(data,length) here, let caller do this
}

#    ifndef VIA_ENABLE

void raw_hid_receive(uint8_t *data, uint8_t length) {
    via_custom_value_command_kb(data, length);
    // uint8_t *command_id        = &(data[0]);
    // uint8_t *channel_id        = &(data[1]);
    // uint8_t *value_id_and_data = &(data[2]);
    // cb_raw_hid_receive_kb(value_id_and_data, length-2);
}

#    endif // VIA_ENABLE

#endif // RAW_ENABLE
