/* Copyright 2025 RyanDam (https://github.com/RyanDam)
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

#include "qp_lvgl.h"

enum coban_pomo_noti_mode_id {
    coban_pomo_noti_mode_none  = 0x00,
    coban_pomo_noti_mode_beep  = 0x01,
    coban_pomo_noti_mode_light = 0x02,
    coban_pomo_noti_mode_both  = 0x03,
};

enum coban_pomo_noti_kind_id {
    coban_pomo_noti_kind_work   = 0x01,
    coban_pomo_noti_kind_rest   = 0x02,
    coban_pomo_noti_kind_cancel = 0x03,
};

enum coban_pomo_state {
    coban_pomo_state_idle = 0x00,
    coban_pomo_state_work = 0x01,
    coban_pomo_state_rest = 0x02,
};

extern bool pomo_running;

void pomo_cb(lv_timer_t *timer);

lv_obj_t *screen_pomodoro_init(void);

void screen_pomodoro_ui_update(void);

void screen_pomodoro_set_time_style(uint8_t time_style);

// Controller

void screen_pomodoro_session_start(void);

void screen_pomodoro_session_cancel(void);

bool screen_pomodoro_session_running(void);

// Event

void screen_pomodoro_session_complete(void);

void screen_pomodoro_set_complete(uint8_t set_idx, bool is_work_set);

void screen_pomodoro_set_start(uint8_t set_idx, bool is_work_set);

// Tas

void screen_pomodoro_stop(void);

void screen_pomodoro_reload(void);
