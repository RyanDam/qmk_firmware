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

#pragma once

#include "qp_lvgl.h"

enum coban_stats_layout_id { coban_stats_layout_2x1 = 0x00, coban_stats_layout_2x2 = 0x01 };

#define STATS_DATA_COUNT 17

enum coban_stats_data_id {
    coban_stats_data_none      = 0x00,
    coban_stats_data_cpu_util  = 0x01,
    coban_stats_data_cpu_temp  = 0x02,
    coban_stats_data_ram       = 0x03,
    coban_stats_data_gpu_util  = 0x04,
    coban_stats_data_gpu_temp  = 0x05,
    coban_stats_data_disk_usage = 0x06,
    coban_stats_data_disk_total = 0x07,
    coban_stats_data_net_sent   = 0x08,
    coban_stats_data_net_recv   = 0x09,
    coban_stats_data_swap_usage = 0x0A,
    coban_stats_data_cpu_freq   = 0x0B,
    coban_stats_data_soc_temp   = 0x0C,
    coban_stats_data_disk_read  = 0x0D,
    coban_stats_data_disk_write = 0x0E,
    coban_stats_data_cpu_power  = 0x0F,
    coban_stats_data_gpu_power  = 0x10,
};

enum coban_stats_unit_id {
    coban_stats_unit_none        = 0x00,
    coban_stats_unit_percent     = 0x01,
    coban_stats_unit_celsius     = 0x02,
    coban_stats_unit_fahrenheit  = 0x03,
    coban_stats_unit_megabytes   = 0x04,
    coban_stats_unit_gigabytes   = 0x05,
    coban_stats_unit_terabytes   = 0x06,
    coban_stats_unit_mbps        = 0x07,
    coban_stats_unit_mhz         = 0x08,
    coban_stats_unit_milliwatts  = 0x09,
};

typedef struct {
    uint16_t value;
    uint16_t max_value;
    uint8_t  unit_id;
} coban_stats_data_t;

lv_obj_t *screen_hardware_stat_init(void);

void screen_hardware_stat_set_data(uint8_t data_id, uint16_t value, uint16_t max_value, uint8_t unit_id);

void screen_hardware_stat_set_layout(uint8_t layout_id);

// void screen_hardware_stat_reload(void);
