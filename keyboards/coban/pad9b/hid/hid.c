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
            uint8_t *offset_big = &(command_data[0]);
            uint8_t *offset_little = &(command_data[1]);
            uint32_t offset = 0x00000000 | (*offset_big << 8) | (*offset_little << 0);
            offset *= 27; // 32 - 2 (via cmd + channel id) - 1 (custom cmd id) - 2 (offsets)

            // screen_time_set_time2(offset);
            // // screen_time_set_datetime(*offset_big, *offset_little, 0, 0, 0);

            uint8_t *data =  &(command_data[2]);
            eeprom_write_block(data, (void *)(EEROM_CB_GIF_ADDR + offset), 27);
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
        }
        case coban_cmd_id_set_gif_buffer: {
            uint8_t *offset_big = &(command_data[0]);
            uint8_t *offset_little = &(command_data[1]);
            uint32_t offset = 0x00000000 | (*offset_big << 8) | (*offset_little << 0);
            offset *= 27; // 32 - 2 (via cmd + channel id) - 1 (custom cmd id) - 2 (offsets)

            eeprom_read_block(command_data+2, (void *)(EEROM_CB_GIF_ADDR + offset), 27);
        }
        default:
            break;
    }
}

void cb_config_save(void) {
    coban_save_config();
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
        switch ( *command_id )
        {
            case id_custom_set_value:
            {
                // cb_config_set_value(value_id_and_data);
                cb_raw_hid_receive_kb(value_id_and_data, length-2);
                break;
            }
            case id_custom_get_value:
            {
                cb_raw_hid_response_kb(value_id_and_data, length-2);
                break;
            }
            case id_custom_save:
            {
                cb_config_save();
                break;
            }
            default:
            {
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
