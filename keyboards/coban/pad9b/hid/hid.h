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
    coban_cmd_id_start = 0x50,
    coban_cmd_id_version = 0x51,
    coban_cmd_id_set_time = 0x52,
    coban_cmd_id_set_cpu_util = 0x53,
    coban_cmd_id_set_gpu_util = 0x54
};

void raw_hid_receive_kb(uint8_t *data, uint8_t length);
