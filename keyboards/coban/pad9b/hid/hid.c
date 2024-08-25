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
#include "hardware/flash.h"
#include <hardware/sync.h>

#define GIF_TRANSFER_BLOCK_SIZE 25

void cb_raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &(data[0]);
    uint8_t *command_data = &(data[1]); // data length: 29

    switch (*command_id) {
        case coban_cmd_id_set_screen: {
            config.screen_idx = *command_data;
            change_screen(config.screen_idx);
            break;
        }
        case coban_cmd_id_set_time: {
            uint8_t *hour = &(command_data[0]);
            uint8_t *minute = &(command_data[1]);
            uint8_t *day = &(command_data[2]);
            uint8_t *month = &(command_data[3]);
            uint8_t *year = &(command_data[4]);
            screen_time_set_datetime(*hour, *minute, *day, *month, *year);
            break;
        }
        case coban_cmd_id_set_cpu_util: {
            screen_hardware_stat_set_cpu(*command_data);
            break;
        }
        case coban_cmd_id_set_gpu_util: {
            screen_hardware_stat_set_gpu(*command_data);
            break;
        }
        case coban_cmd_id_set_time_format: {
            uint8_t *time_style = &(command_data[0]);
            uint8_t *time_format = &(command_data[1]);
            uint8_t *time_indicator = &(command_data[2]);
            uint8_t *date_format = &(command_data[3]);
            uint8_t *date_visibility = &(command_data[4]);
            screen_time_set_format(*time_style, *time_format, *time_indicator, *date_format, *date_visibility);
            break;
        }
        case coban_cmd_id_set_gif_buffer: {
            // first, stop animation to prevent crash because of data writing
            screen_animation_stop();

            uint8_t *offset_1 = &(command_data[0]);
            uint8_t *offset_2 = &(command_data[1]);
            uint8_t *offset_3 = &(command_data[2]);
            uint8_t *offset_4 = &(command_data[3]);
            uint32_t offset = 0x00000000 | (*offset_1 << 24) | (*offset_2 << 16) | (*offset_3 << 8) | (*offset_4 << 0);
            if ((offset + GIF_TRANSFER_BLOCK_SIZE) <= EEPROM_MAX_GIF_SIZE) {
                uint8_t *data =  &(command_data[4]);
                for (uint32_t i = 0; i < GIF_TRANSFER_BLOCK_SIZE; i++) {
                    *(gif_data + offset + i) = *(data + i);
                }
            }
            break;
        }
        case coban_cmd_id_set_gif_size: {
            uint8_t *offset_1 = &(command_data[0]);
            uint8_t *offset_2 = &(command_data[1]);
            uint8_t *offset_3 = &(command_data[2]);
            uint8_t *offset_4 = &(command_data[3]);
            uint32_t gif_datasize = 0x00000000 | (*offset_1 << 24) | (*offset_2 << 16) | (*offset_3 << 8) | (*offset_4 << 0);

            config.gif_data_size = gif_datasize;
            gif_data_header.data_size = config.gif_data_size;
            screen_animation_reload();
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
        default:
            break;
    }
}

void cb_raw_hid_response_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &(data[0]);
    uint8_t *command_data = &(data[1]);

    switch (*command_id) {
        case coban_cmd_id_set_screen: {
            *command_data = config.screen_idx;
            break;
        }
        case coban_cmd_id_set_time_format: {
            *(command_data + 0) = config.time_style_id;
            *(command_data + 1) = config.time_format;
            *(command_data + 2) = config.time_indicator;
            *(command_data + 3) = config.date_format;
            *(command_data + 4) = config.date_visibility;
            break;
        }
        case coban_cmd_id_set_gif_buffer: {
            uint8_t *offset_1 = &(command_data[0]);
            uint8_t *offset_2 = &(command_data[1]);
            uint8_t *offset_3 = &(command_data[2]);
            uint8_t *offset_4 = &(command_data[3]);
            uint32_t offset = 0x00000000 | (*offset_1 << 24) | (*offset_2 << 16) | (*offset_3 << 8) | (*offset_4 << 0);

            if ((offset + GIF_TRANSFER_BLOCK_SIZE) <= EEPROM_MAX_GIF_SIZE) {
                for (uint32_t i = 0; i < GIF_TRANSFER_BLOCK_SIZE; i++) {
                    *(command_data + i + 4) = *(gif_data + offset + i);
                }
            }
            break;
        }
        case coban_cmd_id_set_gif_size: {
            *(command_data + 0) = (uint8_t) ((config.gif_data_size & 0xff000000) >> 24);
            *(command_data + 1) = (uint8_t) ((config.gif_data_size & 0x00ff0000) >> 16);
            *(command_data + 2) = (uint8_t) ((config.gif_data_size & 0x0000ff00) >> 8);
            *(command_data + 3) = (uint8_t) ((config.gif_data_size & 0x000000ff) >> 0);
            break;
        }
        case coban_cmd_id_set_gif_flash: {
            uint8_t *offset_1 = &(command_data[0]);
            uint8_t *offset_2 = &(command_data[1]);
            uint8_t *offset_3 = &(command_data[2]);
            uint8_t *offset_4 = &(command_data[3]);
            uint32_t offset = 0x00000000 | (*offset_1 << 24) | (*offset_2 << 16) | (*offset_3 << 8) | (*offset_4 << 0);

            if ((offset + GIF_TRANSFER_BLOCK_SIZE) <= EEPROM_MAX_GIF_SIZE) {
                const uint8_t *pointer = (const uint8_t *) (XIP_BASE + EEROM_CB_GIF_ADDR + offset);
                for (int i = 0; i < GIF_TRANSFER_BLOCK_SIZE; i++) {
                    *(command_data + i + 4) = *(pointer + i);
                }
            }
            break;
        }
        default:
            break;
    }
}

void cb_config_save(void) {
    coban_save_config();
    // save gif to flash
    uint32_t ints = save_and_disable_interrupts();
    // Calculate the absolute address in flash memory
    if (EEROM_CB_GIF_ADDR + config.gif_data_size <= PICO_FLASH_SIZE_BYTES) {
        // Write data to flash
        int plus_erase = config.gif_data_size % FLASH_SECTOR_SIZE > 0 ? 1 : 0;
        size_t datasize_erase = (((size_t) config.gif_data_size / FLASH_SECTOR_SIZE) + plus_erase) * FLASH_SECTOR_SIZE;
        // first, earse target flash is required befor write data to it
        flash_range_erase(EEROM_CB_GIF_ADDR, datasize_erase);

        // Write data to flash
        int plus_program = config.gif_data_size % FLASH_PAGE_SIZE > 0 ? 1 : 0;
        size_t datasize_program = (((size_t) config.gif_data_size / FLASH_PAGE_SIZE) + plus_program) * FLASH_PAGE_SIZE;
        // program flash
        flash_range_program(EEROM_CB_GIF_ADDR, gif_data, datasize_program);
    } else {
        // Handle error: offset exceeds flash size
    }
    restore_interrupts (ints);
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

    if ( *channel_id == id_custom_channel ) {
        switch ( *command_id ) {
            case id_custom_set_value: {
                // cb_config_set_value(value_id_and_data);
                cb_raw_hid_receive_kb(value_id_and_data, length-2);
                break;
            }
            case id_custom_get_value: {
                cb_raw_hid_response_kb(value_id_and_data, length-2);
                break;
            }
            case id_custom_save: {
                cb_config_save();
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

#ifndef VIA_ENABLE

void raw_hid_receive(uint8_t *data, uint8_t length) {
    via_custom_value_command_kb(data, length);
    // uint8_t *command_id        = &(data[0]);
    // uint8_t *channel_id        = &(data[1]);
    // uint8_t *value_id_and_data = &(data[2]);
    // cb_raw_hid_receive_kb(value_id_and_data, length-2);
}

#endif // VIA_ENABLE

#endif // RAW_ENABLE
