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
#include "graphics/screens/screen_hardware_stats.h"
#include "graphics/screens/screen_time.h"

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &(data[0]);
    uint8_t *command_data = &(data[1]);

    switch (*command_id)
    {
    case coban_cmd_id_version: {

        break;
    }
    case coban_cmd_id_set_time: {
        uint8_t *hour = &(command_data[0]);
        uint8_t *minute = &(command_data[1]);
        uint8_t *day = &(command_data[2]);
        uint8_t *month = &(command_data[3]);
        uint8_t *year = &(command_data[4]);
        screen_time_set_time(*hour, *minute, *day, *month, *year);
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
    default:
        break;
    }
}

#ifdef RAW_ENABLE

#ifdef VIA_ENABLE

bool check_if_via_cmd(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &(data[0]);
    if (command_id >= coban_cmd_id_start) {
        return false;
    }
    return true;
}

bool via_command_kb(uint8_t *data, uint8_t length) {
    if (check_if_via_cmd(data, length)) {
        return false;
    }
    raw_hid_receive_kb(data, length);
    return true;
}

#else // VIA_ENABLE

void raw_hid_receive(uint8_t *data, uint8_t length) {
    raw_hid_receive_kb(data, length);
}

#endif // VIA_ENABLE

#endif // RAW_ENABLE
