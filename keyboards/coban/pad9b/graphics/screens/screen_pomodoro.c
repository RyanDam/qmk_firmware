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

#include "graphics/screens/screen_pomodoro.h"
#include "graphics/screens/styles.h"
#include "graphics/lvgl_helpers.h"

#include "eeprom/cb_eeprom.h"

static lv_obj_t *screen_pomodoro = NULL;
static lv_timer_t *pomo_timer = NULL;
static bool pomo_running = false;

/* Canvas buffer */
#define CANVAS_POMO_WIDTH SCREEN_WIDTH
#define CANVAS_POMO_HEIGHT 6
static lv_obj_t * pomo_layout_holder = NULL;
static lv_obj_t * pomo_progress_holder = NULL;
static lv_obj_t * pomo_indice_holder = NULL;
static lv_obj_t * pomo_indice = NULL;
static lv_obj_t * pomo_time_holder = NULL;
static lv_obj_t * pomo_time_text = NULL;
static lv_obj_t * pomo_time_status = NULL;

static lv_style_t style_progress_work;
static lv_style_t style_progress_rest;
static lv_style_t style_progress_curr;
static lv_style_t style_progress_past;
static lv_style_t style_time_indice;

static bool session_running = false;
static uint32_t session_start_timestamp = 0;

static int pomo_total_set_num;
static uint32_t pomo_total_session_duration_min;
static int pomo_work_set_width;
static int pomo_rest_set_width;
static uint8_t pomo_current_set_idx = 0; // default not zero to ensure set event is triggered

lv_obj_t * screen_pomodoro_init(void) {
    screen_pomodoro = lv_obj_create(NULL);
    lv_obj_add_style(screen_pomodoro, &style_screen, 0);
    // lv_obj_set_style_bg_color(screen_pomodoro, lv_color_hex(0xffff00), 0);

    pomo_layout_holder = lv_obj_create(screen_pomodoro);
    lv_obj_add_style(pomo_layout_holder, &style_container, 0);
    // lv_obj_set_style_bg_color(pomo_layout_holder, lv_color_hex(0x00ff00), 0);
    lv_obj_center(pomo_layout_holder);
    use_flex_column(pomo_layout_holder);
    lv_obj_set_style_pad_row(pomo_layout_holder, 0, 0);

    pomo_time_holder = lv_obj_create(pomo_layout_holder);  // attach to active screen
    lv_obj_add_style(pomo_time_holder, &style_container, 0);
    lv_obj_set_size(pomo_time_holder, SCREEN_WIDTH, SCREEN_HEIGHT-CANVAS_POMO_HEIGHT*2);
    use_flex_column(pomo_time_holder);
    lv_obj_set_style_pad_row(pomo_time_holder, 0, 0);
    // lv_obj_set_style_bg_opa(pomo_time_holder, LV_OPA_100, 0);
    // lv_obj_set_style_bg_color(pomo_time_holder, lv_color_hex(0x0000ff), 0);

    pomo_time_status = lv_label_create(pomo_time_holder);
    lv_label_set_text(pomo_time_status, "RESTING");
    lv_obj_add_style(pomo_time_status, &style_text, 0);

    pomo_time_text = lv_label_create(pomo_time_holder);
    lv_label_set_text(pomo_time_text, "--:--");
    lv_obj_add_style(pomo_time_text, &style_text_time1, 0);
    lv_obj_add_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);

    pomo_indice_holder = lv_obj_create(pomo_layout_holder);  // attach to active screen
    lv_obj_add_style(pomo_indice_holder, &style_container, 0);
    lv_obj_set_size(pomo_indice_holder, SCREEN_WIDTH, CANVAS_POMO_HEIGHT);
    lv_obj_set_style_pad_column(pomo_indice_holder, 0, 0);

    pomo_progress_holder = lv_obj_create(pomo_layout_holder);  // attach to active screen
    lv_obj_add_style(pomo_progress_holder, &style_container, 0);
    lv_obj_set_size(pomo_progress_holder, SCREEN_WIDTH, CANVAS_POMO_HEIGHT);
    lv_obj_set_style_pad_column(pomo_progress_holder, 0, 0);
    // lv_obj_set_style_bg_opa(pomo_progress_holder, LV_OPA_100, 0);
    // lv_obj_set_style_bg_color(pomo_progress_holder, lv_color_hex(0xff0000), 0);
    use_flex_row(pomo_progress_holder);
    lv_obj_set_flex_align(pomo_progress_holder, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    screen_pomodoro_ui_update();

    return screen_pomodoro;
}

void screen_pomodoro_ui_update(void) {
    if (pomo_progress_holder == NULL) {
        return;
    }

    if (config.pomo_num_set == 0) config.pomo_num_set = 4;
    if (config.pomo_work_duration == 0) config.pomo_work_duration = 25;
    if (config.pomo_rest_duration == 0) config.pomo_rest_duration = 5;

    pomo_total_set_num = 2*config.pomo_num_set - 1;
    pomo_total_session_duration_min = config.pomo_work_duration * config.pomo_num_set + config.pomo_rest_duration * (config.pomo_num_set - 1);
    pomo_work_set_width = (int) (CANVAS_POMO_WIDTH * (((float) config.pomo_work_duration) / ((float) pomo_total_session_duration_min)));
    pomo_rest_set_width = (int) (CANVAS_POMO_WIDTH * (((float) config.pomo_rest_duration) / ((float) pomo_total_session_duration_min)));

    lv_style_init(&style_progress_work);
    lv_style_set_border_width(&style_progress_work, 1);
    lv_style_set_width(&style_progress_work, pomo_work_set_width);
    lv_style_set_height(&style_progress_work, CANVAS_POMO_HEIGHT);
    lv_style_set_bg_opa(&style_progress_work, LV_OPA_100);
    lv_style_set_bg_color(&style_progress_work, lv_color_hex(0x555555));
    lv_style_set_border_color(&style_progress_work, lv_color_hex(0x555555));
    lv_style_set_radius(&style_progress_work, 2);

    lv_style_init(&style_progress_rest);
    lv_style_set_border_width(&style_progress_rest, 1);
    lv_style_set_width(&style_progress_rest, pomo_rest_set_width);
    lv_style_set_height(&style_progress_rest, CANVAS_POMO_HEIGHT);
    lv_style_set_bg_opa(&style_progress_rest, LV_OPA_100);
    lv_style_set_bg_color(&style_progress_rest, lv_color_hex(0x252525));
    lv_style_set_border_color(&style_progress_rest, lv_color_hex(0x252525));
    lv_style_set_radius(&style_progress_rest, 2);

    lv_style_init(&style_progress_curr);
    lv_style_set_border_width(&style_progress_curr, 1);
    lv_style_set_width(&style_progress_curr, pomo_work_set_width);
    lv_style_set_height(&style_progress_curr, CANVAS_POMO_HEIGHT);
    lv_style_set_bg_opa(&style_progress_curr, LV_OPA_100);
    lv_style_set_bg_color(&style_progress_curr, lv_color_hex(0x0000dd));
    lv_style_set_border_color(&style_progress_curr, lv_color_hex(0x0000dd));
    lv_style_set_radius(&style_progress_curr, 2);

    lv_style_init(&style_progress_past);
    lv_style_set_border_width(&style_progress_past, 1);
    lv_style_set_width(&style_progress_past, pomo_work_set_width);
    lv_style_set_height(&style_progress_past, CANVAS_POMO_HEIGHT);
    lv_style_set_bg_opa(&style_progress_past, LV_OPA_100);
    lv_style_set_bg_color(&style_progress_past, lv_color_hex(0xdddd00));
    lv_style_set_border_color(&style_progress_past, lv_color_hex(0xdddd00));
    lv_style_set_radius(&style_progress_past, 2);

    lv_style_init(&style_time_indice);
    lv_style_set_pad_top(&style_time_indice, 0);
    lv_style_set_pad_bottom(&style_time_indice, 0);
    lv_style_set_pad_left(&style_time_indice, 0);
    lv_style_set_pad_right(&style_time_indice, 0);
    lv_style_set_border_width(&style_time_indice, 1);
    lv_style_set_width(&style_time_indice, 4);
    lv_style_set_height(&style_time_indice, CANVAS_POMO_HEIGHT);
    lv_style_set_bg_opa(&style_time_indice, LV_OPA_100);
    lv_style_set_bg_color(&style_time_indice, lv_color_hex(0xffffff));
    lv_style_set_border_color(&style_time_indice, lv_color_hex(0xffffff));

    pomo_indice = lv_obj_create(pomo_indice_holder);  // attach to active screen
    lv_obj_add_style(pomo_indice, &style_time_indice, 0);
    lv_obj_set_x(pomo_indice, 0);

    // remove all progress views
    lv_obj_t * child = lv_obj_get_child(pomo_progress_holder, 0);
    while(child) {
        lv_obj_del(child);
        child = lv_obj_get_child(pomo_progress_holder, 0);
    }

    // calculate and add new progress view
    int idx = 0;
    int width = pomo_work_set_width;
    lv_style_t * style;
    for (idx = 0; idx < pomo_total_set_num; idx++) {
        if (idx % 2 == 0) {
            // work set
            style = &style_progress_work;
            width = pomo_work_set_width;
        } else {
            // rest set
            style = &style_progress_rest;
            width = pomo_rest_set_width;
        }
        lv_obj_t *pomo_progress_block = lv_obj_create(pomo_progress_holder);
        lv_obj_add_style(pomo_progress_block, style, 0);
        lv_obj_set_style_width(pomo_progress_block, width, 0);
    }

    // lv_label_set_text_fmt(pomo_time_status, "%03d\n%03d\n%03d", pomo_work_set_width, pomo_rest_set_width, pomo_total_set_num);
}

void screen_pomodoro_session_start(void) {
    session_start_timestamp = timer_read32();
    pomo_current_set_idx = 0;
    lv_obj_set_x(pomo_indice, 0);
    lv_obj_clear_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(pomo_indice, LV_OBJ_FLAG_HIDDEN);
    session_running = true;
}

void screen_pomodoro_session_cancel(void) {
    session_running = false;
}

bool screen_pomodoro_session_running(void) {
    return session_running;
}

void screen_pomodoro_session_complete(void) {
    session_running = false;

    // TODO: ring a session completion tone
}

void screen_pomodoro_set_complete(uint8_t set_idx, bool is_work_set) {
    // TODO: ring a set completion tone
    lv_obj_add_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(pomo_indice, LV_OBJ_FLAG_HIDDEN);
}

void screen_pomodoro_set_start(uint8_t set_idx, bool is_work_set) {

}

static void pomo_cb(lv_timer_t * timer) {
    if (!pomo_running) {
        return;
    }

    if (!session_running) {
        // No pomo session yet
        lv_label_set_text(pomo_time_text, "--:--");
        lv_label_set_text(pomo_time_status, "RESTING");
        return;
    }

    uint32_t current_timestamp = timer_read32();
    uint32_t elapsed_time_sec = (current_timestamp - session_start_timestamp)/1000;

    if (elapsed_time_sec >= pomo_total_session_duration_min * 60) {
        // session is completed
        screen_pomodoro_session_complete();
        return;
    }

    // Calculate the current stat
    uint32_t set_time_start_sec = 0;
    uint32_t set_time_left_sec = 0;
    uint8_t set_idx = 0;
    for (int idx = 0; idx < pomo_total_set_num; idx++) {
        uint32_t set_duration_sec = (idx % 2 == 0 ? config.pomo_work_duration : config.pomo_rest_duration)*60;

        uint32_t set_time_end_sec = set_time_start_sec + set_duration_sec;
        if (elapsed_time_sec >= set_time_start_sec && elapsed_time_sec < set_time_end_sec) {
            // this is the current set
            set_idx = idx;
            set_time_left_sec = set_duration_sec - (elapsed_time_sec - set_time_start_sec);
        }

        // advance set parameter;
        set_time_start_sec = set_time_end_sec;
    }

    // Check if set changed
    if (pomo_current_set_idx != set_idx) {
        // set changed, trigger set event
        screen_pomodoro_set_complete(pomo_current_set_idx, pomo_current_set_idx % 2 == 0);
        screen_pomodoro_set_start(set_idx, set_idx % 2 == 0);
    }
    pomo_current_set_idx = set_idx;

    // Update UI
    uint8_t time_left_minute = set_time_left_sec / 60;
    uint8_t time_left_second = set_time_left_sec % 60;
    lv_label_set_text_fmt(pomo_time_text, "%02d:%02d", time_left_minute, time_left_second);
    if (set_idx % 2 == 0) {
        lv_label_set_text(pomo_time_status, "FOCUS");
    } else {
        lv_label_set_text(pomo_time_status, "RELAX");
    }

    float progress_percentage = ((float)elapsed_time_sec) / ((float)pomo_total_session_duration_min * 60);
    int indice_x = SCREEN_WIDTH*progress_percentage;
    lv_obj_set_x(pomo_indice, indice_x);

    // // update progress block UI
    // for (int idx = 0; idx < pomo_total_set_num; idx++) {
    //     uint32_t set_width = idx % 2 == 0 ? pomo_work_set_width : pomo_rest_set_width;
    //     // render
    //     lv_obj_t * progress_block = lv_obj_get_child(pomo_progress_holder, idx);
    //     if (idx == pomo_current_set_idx) {
    //         // current block
    //         lv_obj_add_style(progress_block, &style_progress_curr, 0);
    //     } else if (idx < pomo_current_set_idx) {
    //         // previous block
    //         lv_obj_add_style(progress_block, &style_progress_past, 0);
    //     } else if (idx % 2 == 0) {
    //         // work set
    //         lv_obj_add_style(progress_block, &style_progress_work, 0);
    //     } else {
    //         // rest set
    //         lv_obj_add_style(progress_block, &style_progress_rest, 0);
    //     }
    //     lv_obj_set_style_width(progress_block, set_width, 0);
    // }
}

void screen_pomodoro_stop(void) {
    if (screen_pomodoro == NULL) return;
    if (!pomo_running) {
        return;
    }
    pomo_running = false;
    lv_timer_pause(pomo_timer);
}

void screen_pomodoro_reload(void) {
    if (screen_pomodoro == NULL) return;
    if (pomo_running) {
        return;
    }
    pomo_running = true;
    if (pomo_timer == NULL) {
        // 1 FPS
        pomo_timer = lv_timer_create(pomo_cb, 1000, NULL);
    } else {
        lv_timer_resume(pomo_timer);
    }
}
