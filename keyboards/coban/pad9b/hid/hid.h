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

#include "raw_hid.h"

enum coban_command_id { // via start at 0x00, currently end at 0x15
    coban_cmd_id_start           = 0x50,
    coban_cmd_id_set_screen      = 0x51,
    coban_cmd_id_set_time        = 0x52,
    coban_cmd_id_set_cpu_util    = 0x53,
    coban_cmd_id_set_gpu_util    = 0x54,
    coban_cmd_id_set_time_format = 0x55,
    coban_cmd_id_set_gif_buffer  = 0x56,
    coban_cmd_id_set_gif_size    = 0x57,
    coban_cmd_id_set_gif_flash   = 0x58,

    coban_cmd_id_reboot_board    = 0x98,
    coban_cmd_id_reset_board     = 0x99
};

#ifndef VIA_ENABLE

enum via_command_id {
    id_custom_set_value       = 0x07,
    id_custom_get_value       = 0x08,
    id_custom_save            = 0x09,
    id_unhandled              = 0xff
};

enum via_channel_id {
    id_custom_channel         = 0x00,
};

#endif // VIA_ENABLE

void raw_hid_receive_kb(uint8_t *data, uint8_t length);
