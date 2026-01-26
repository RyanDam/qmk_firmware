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

#include <math.h>
#include "audio/audio.h"
#include "graphics/screens/screen_pomodoro.h"
#include "graphics/screens/screen_time.h"
#include "graphics/screens/styles.h"
#include "graphics/lvgl_helpers.h"

#include "eeprom/cb_eeprom.h"

static lv_obj_t *screen_pomodoro = NULL;
// static lv_timer_t *pomo_timer = NULL;
bool pomo_running = false;
static enum coban_pomo_state pomo_state = coban_pomo_state_idle;

/* Canvas buffer */
#define CANVAS_POMO_WIDTH SCREEN_WIDTH
#define CANVAS_POMO_HEIGHT 6
static lv_obj_t * pomo_layout_holder = NULL;
static lv_obj_t * pomo_progress_bar = NULL;
static lv_obj_t * pomo_time_holder = NULL;
static lv_obj_t * pomo_time_text = NULL;
static lv_obj_t * pomo_time_status = NULL;
static lv_obj_t * pomo_breath_indicator = NULL;

static uint32_t session_start_timestamp = 0;

static int pomo_total_set_num;
static uint32_t pomo_total_session_duration_min;
static int pomo_work_set_width;
static int pomo_rest_set_width;
static uint8_t pomo_current_set_idx = 0; // default not zero to ensure set event is triggered
static int quote_offset;

// animation variables
static uint32_t pomo_breath_size = 60;

// notify variables
static uint32_t pomo_noti_start_timestamp = 0;
static uint32_t pomo_noti_duration_ms = 3000;
static bool pomo_noti_restored = false;
// #ifdef RGB_MATRIX_ENABLE
// static int last_rgb_matrix_effect;
// #endif

#ifdef AUDIO_ENABLE
#define SIMPLE_ALARM_SOUND Q__NOTE(_C6), Q__NOTE(_C6),
#define SIMPLE_ALARM_LONG_SOUND Q__NOTE(_C6), Q__NOTE(_C6), B__NOTE(_REST), Q__NOTE(_C6), Q__NOTE(_C6), B__NOTE(_REST), Q__NOTE(_C6), Q__NOTE(_C6),
float work_song[][2] = SONG(SIMPLE_ALARM_LONG_SOUND);
float rest_song[][2] = SONG(SIMPLE_ALARM_LONG_SOUND);
float cancel_song[][2] = SONG(SIMPLE_ALARM_SOUND);
#endif

char * get_current_quote(void);

lv_obj_t * screen_pomodoro_init(void) {
    quote_offset = rand();

    screen_pomodoro = lv_obj_create(NULL);
    lv_obj_add_style(screen_pomodoro, &style_screen, 0);

    pomo_breath_indicator = lv_obj_create(screen_pomodoro);
    lv_obj_add_style(pomo_breath_indicator, &style_container, 0);
    lv_obj_center(pomo_breath_indicator);
    lv_obj_set_style_bg_opa(pomo_breath_indicator, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(pomo_breath_indicator, lv_color_hex(0x005639), 0);
    lv_obj_set_size(pomo_breath_indicator, pomo_breath_size, pomo_breath_size);
    lv_obj_set_style_radius(pomo_breath_indicator, pomo_breath_size/2, 0);

    pomo_layout_holder = lv_obj_create(screen_pomodoro);
    lv_obj_add_style(pomo_layout_holder, &style_container, 0);
    lv_obj_center(pomo_layout_holder);
    use_flex_column(pomo_layout_holder);
    lv_obj_set_style_pad_row(pomo_layout_holder, 0, 0);
    lv_obj_set_style_bg_opa(pomo_layout_holder, LV_OPA_TRANSP, 0);


    // main time + status holder
    pomo_time_holder = lv_obj_create(pomo_layout_holder);
    lv_obj_add_style(pomo_time_holder, &style_container, 0);
    lv_obj_set_size(pomo_time_holder, SCREEN_WIDTH, SCREEN_HEIGHT-CANVAS_POMO_HEIGHT);
    use_flex_column(pomo_time_holder);
    lv_obj_set_style_pad_row(pomo_time_holder, 6, 0);
    lv_obj_set_flex_align(pomo_time_holder, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_obj_set_style_bg_opa(pomo_time_holder, LV_OPA_TRANSP, 0);

    pomo_time_status = lv_label_create(pomo_time_holder);
    lv_label_set_text(pomo_time_status, "Có chí thì nên");
    lv_obj_add_style(pomo_time_status, &style_text, 0);
    lv_label_set_long_mode(pomo_time_status, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(pomo_time_status, SCREEN_WIDTH - 16);
    lv_obj_set_style_text_align(pomo_time_status, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(pomo_time_status, LV_OPA_TRANSP, 0);

    pomo_time_text = lv_label_create(pomo_time_holder);
    lv_label_set_text(pomo_time_text, "--:--");
    lv_obj_add_style(pomo_time_text, &style_text_time1, 0);
    lv_obj_add_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_opa(pomo_time_text, LV_OPA_TRANSP, 0);

    // Progress holder
    pomo_progress_bar = lv_bar_create(pomo_layout_holder);
    lv_obj_add_style(pomo_progress_bar, &style_container, 0);
    lv_obj_set_size(pomo_progress_bar, SCREEN_WIDTH, CANVAS_POMO_HEIGHT);
    lv_obj_set_style_bg_color(pomo_progress_bar, lv_color_hex(0x252525), 0);
    lv_obj_set_style_bg_opa(pomo_progress_bar, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(pomo_progress_bar, lv_color_hex(0x009664), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(pomo_progress_bar, LV_OPA_100, LV_PART_INDICATOR);
    lv_bar_set_range(pomo_progress_bar, 0, 100);
    lv_bar_set_value(pomo_progress_bar, 25, LV_ANIM_OFF);
    lv_obj_add_flag(pomo_progress_bar, LV_OBJ_FLAG_HIDDEN);


    screen_pomodoro_ui_update();
    screen_pomodoro_set_time_style(config.time_style_id);

    return screen_pomodoro;
}

void screen_pomodoro_ui_update(void) {
    if (config.pomo_num_set == 0) config.pomo_num_set = 4;
    if (config.pomo_work_duration == 0) config.pomo_work_duration = 25;
    if (config.pomo_rest_duration == 0) config.pomo_rest_duration = 5;

    pomo_total_set_num = 2*config.pomo_num_set - 1;
    pomo_total_session_duration_min = config.pomo_work_duration * config.pomo_num_set + config.pomo_rest_duration * (config.pomo_num_set - 1);
    pomo_work_set_width = (int) (CANVAS_POMO_WIDTH * (((float) config.pomo_work_duration) / ((float) pomo_total_session_duration_min)));
    pomo_rest_set_width = (int) (CANVAS_POMO_WIDTH * (((float) config.pomo_rest_duration) / ((float) pomo_total_session_duration_min)));
}

void screen_pomodoro_set_time_style(uint8_t time_style) {
    config.time_style_id = time_style;

    switch (config.time_style_id) {
        case coban_time_style_2: {
            lv_obj_add_style(pomo_time_text, &style_text_time2, 0);
            break;
        }
        case coban_time_style_3: {
            lv_obj_add_style(pomo_time_text, &style_text_time3, 0);
            break;
        }
        case coban_time_style_1:
        default: {
            lv_obj_add_style(pomo_time_text, &style_text_time1, 0);
            break;
        }
    }
}

void screen_pomodoro_do_notify(enum coban_pomo_noti_kind_id noti_kind_id) {
    if (config.pomo_noti_mode == coban_pomo_noti_mode_beep || config.pomo_noti_mode == coban_pomo_noti_mode_both) {
#ifdef AUDIO_ENABLE
        stop_all_notes();
        switch (noti_kind_id) {
            case coban_pomo_noti_kind_work: {
                PLAY_SONG(work_song);
                break;
            }
            case coban_pomo_noti_kind_rest: {
                PLAY_SONG(rest_song);
                break;
            }
            case coban_pomo_noti_kind_cancel: {
                PLAY_SONG(cancel_song);
                break;
            }
            default: {
                break;
            }
        }
#endif // AUDIO_ENABLE
    }
    if (config.pomo_noti_mode == coban_pomo_noti_mode_light || config.pomo_noti_mode == coban_pomo_noti_mode_both) {
        pomo_noti_start_timestamp = timer_read32();
        pomo_noti_restored = false;
        switch (noti_kind_id) {
            case coban_pomo_noti_kind_work: {
                pomo_noti_duration_ms = 3000;
                break;
            }
            case coban_pomo_noti_kind_rest: {
                pomo_noti_duration_ms = 3000;
                break;
            }
            case coban_pomo_noti_kind_cancel: {
                pomo_noti_duration_ms = 800;
                break;
            }
            default: {
                break;
            }
        }
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_enable_noeeprom();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_screen_pomo_noti_effect);
#endif // RGB_MATRIX_ENABLE
    }
}

void screen_pomodoro_notify_task(void) {
    uint32_t current_timestamp = timer_read32();
    uint32_t elapsed_time = current_timestamp - pomo_noti_start_timestamp;
    bool is_finished = (elapsed_time >= pomo_noti_duration_ms);
#ifdef RGB_MATRIX_ENABLE
    if (is_finished) {
        if (!pomo_noti_restored) {
            rgb_matrix_reload_from_eeprom();
            pomo_noti_restored = true;
        }
    } else {
        // do nothing
    }
#endif // RGB_MATRIX_ENABLE
}

void screen_pomodoro_session_start(void) {
    session_start_timestamp = timer_read32();
    pomo_current_set_idx = 0;
    // lv_obj_set_x(pomo_indice, 0);
    lv_obj_clear_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(pomo_progress_bar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(pomo_breath_indicator, LV_OBJ_FLAG_HIDDEN);
    pomo_state = coban_pomo_state_work;

    screen_pomodoro_do_notify(coban_pomo_noti_kind_work);
}

void screen_pomodoro_session_cancel(void) {
    pomo_state = coban_pomo_state_idle;
    lv_obj_add_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(pomo_progress_bar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(pomo_breath_indicator, LV_OBJ_FLAG_HIDDEN);

    screen_pomodoro_do_notify(coban_pomo_noti_kind_cancel);
}

bool screen_pomodoro_session_running(void) {
    return pomo_state != coban_pomo_state_idle;
}

void screen_pomodoro_session_complete(void) {
    pomo_state = coban_pomo_state_idle;
    lv_obj_add_flag(pomo_time_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(pomo_progress_bar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(pomo_breath_indicator, LV_OBJ_FLAG_HIDDEN);

    screen_pomodoro_do_notify(coban_pomo_noti_kind_rest);
}

void screen_pomodoro_set_complete(uint8_t set_idx, bool is_work_set) {
    // TODO: ring a set completion tone
}

void screen_pomodoro_set_start(uint8_t set_idx, bool is_work_set) {
    if (is_work_set) {
        pomo_state = coban_pomo_state_work;
        lv_obj_add_flag(pomo_breath_indicator, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(pomo_progress_bar, LV_OBJ_FLAG_HIDDEN);
        screen_pomodoro_do_notify(coban_pomo_noti_kind_work);
    } else { // rest set
        pomo_state = coban_pomo_state_rest;
        lv_obj_clear_flag(pomo_breath_indicator, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(pomo_progress_bar, LV_OBJ_FLAG_HIDDEN);
        screen_pomodoro_do_notify(coban_pomo_noti_kind_rest);
    }
}

static inline double smoothstep(double x) {
    // Clamp 0..1
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x * x * (3.0 - 2.0 * x);
}

// Continuous breathing waveform
double breathing_wave(double t, double T_in, double T_hold, double T_out, double T_hold2) {
    double T = T_in + T_hold + T_out + T_hold2;
    double phase = fmod(t, T);

    // Inhale phase
    if (phase < T_in) {
        double x = phase / T_in;
        // Smooth -1 -> +1
        return -1.0 + 2.0 * smoothstep(x);
    }

    // Hold at top (smooth transition from inhale)
    else if (phase < T_in + T_hold) {
        double x = (phase - T_in) / T_hold;
        // Stay near +1 but slightly smooth at edges
        return 1.0 - 0.02 * (1.0 - cos(M_PI * x)); // tiny oscillation for realism
    }

    // Exhale phase
    else if (phase < T_in + T_hold + T_out) {
        double x = (phase - T_in - T_hold) / T_out;
        // Smooth +1 -> -1
        return 1.0 - 2.0 * smoothstep(x);
    }

    // Hold at bottom (smooth transition from exhale)
    else {
        double x = (phase - T_in - T_hold - T_out) / T_hold2;
        return -1.0 + 0.02 * (1.0 - cos(M_PI * x));
    }
}

void pomo_cb(lv_timer_t * timer) {
    if (!pomo_running) {
        return;
    }

    screen_pomodoro_notify_task();

    uint32_t current_timestamp = timer_read32();

    // calculate breath cycle state
    float breath_progress_percentage = (breathing_wave((float)current_timestamp/1000, 3, 0.5, 3, 0.5) + 1 ) / 2; // from 0 to 1
    pomo_breath_size = (uint32_t) (breath_progress_percentage * (64-28) + 28);  // map to 28 - 64
    lv_obj_set_size(pomo_breath_indicator, pomo_breath_size, pomo_breath_size);
    lv_obj_set_style_radius(pomo_breath_indicator, pomo_breath_size/2, 0);

    if (pomo_state == coban_pomo_state_idle) {
        // No pomo session yet
        lv_label_set_text(pomo_time_text, "--:--");
        lv_label_set_text(pomo_time_status, get_current_quote());
        return;
    }


    uint32_t elapsed_time_sec = (current_timestamp - session_start_timestamp)/1000;
    if (elapsed_time_sec >= pomo_total_session_duration_min * 60) {
        // session is completed
        screen_pomodoro_session_complete();
        return;
    }

    // Calculate the current stat
    uint32_t set_time_start_sec = 0;
    uint32_t set_time_left_sec = 0;
    uint32_t set_time_total_sec = 0;
    uint8_t set_idx = 0;
    for (int idx = 0; idx < pomo_total_set_num; idx++) {
        uint32_t set_duration_sec = (idx % 2 == 0 ? config.pomo_work_duration : config.pomo_rest_duration)*60;

        uint32_t set_time_end_sec = set_time_start_sec + set_duration_sec;
        if (elapsed_time_sec >= set_time_start_sec && elapsed_time_sec < set_time_end_sec) {
            // this is the current set
            set_idx = idx;
            set_time_left_sec = set_duration_sec - (elapsed_time_sec - set_time_start_sec);
            set_time_total_sec = set_duration_sec;
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
        lv_label_set_text_fmt(pomo_time_status, "FOCUS #%d", (int)(set_idx/2) + 1);
    } else {
        lv_label_set_text(pomo_time_status, get_current_quote());
    }

    float set_progress_percentage = 1.0 - (((float)set_time_left_sec) / ((float)set_time_total_sec));
    lv_bar_set_value(pomo_progress_bar, (int32_t) (set_progress_percentage * 100), false);
}

void screen_pomodoro_stop(void) {
    if (screen_pomodoro == NULL) return;
    if (screen_pomodoro_session_running()) {
        // if pomodoro session is already running, keep timer running
        return;
    }
    if (!pomo_running) {
        return;
    }
    pomo_running = false;

    // also cancel any ongoing pomo notify
    if (!pomo_noti_restored) {
        rgb_matrix_reload_from_eeprom();
        pomo_noti_restored = true;
    }
}

void screen_pomodoro_reload(void) {
    if (screen_pomodoro == NULL) return;
    if (pomo_running) {
        return;
    }
    pomo_running = true;
}

const char* quotes[] = {
    "Có công mài sắt\ncó ngày nên kim",
    "Kiến tha lâu\ncũng đầy tổ",
    "Có chí thì nên",
    "Công thành\nchẳng quản lâu",
    "Học thầy không tày học bạn",
    "Đi một ngày đàng\nhọc một sàng khôn",
    "Uống nước\nnhớ nguồn",
    "Ăn quả\nnhớ kẻ trồng cây",
    "Đói cho sạch\nrách cho thơm",
    "Tốt gỗ\nhơn tốt nước sơn",
    "Thất bại\nlà mẹ thành công",
    "Cây ngay\nkhông lo chết đứng",
    "Biết người biết ta\ntrăm trận trăm thắng",
    "Gần mực thì đen\ngần đèn thì sáng",
    "Chân cứng đá mềm",
    "Tiền nào của nấy",
    "Không thầy\nđố mày làm nên",
    "Học ăn, học nói\nhọc gói, học mở",
    "Hữu chí\ncánh thành",
    "Lửa thử vàng\ngian nan thử sức",
    "Thua keo này\nta bày keo khác",
    "Được mùa chớ phụ ngô khoai",
    "Giấy rách\nphải giữ lấy lề",
    "Đèn nhà ai nấy rạng",
    "Ở hiền gặp lành",
    "Có chí làm quan\ncó gan làm giàu",
    "Còn nước, còn tát",
    "Trời sinh voi\ntrời sinh cỏ",
    "Nước chảy đá mòn",
    "Cái khó\nló cái khôn",
    "Cần cù\nbù thông minh",
    "Ăn vóc học hay",
    "Học một biết mười",
    "Góp gió thành bão",
    "Học đi đôi với hành",
    "Tre già măng mọc",
    "Khó khăn\nthử thách lòng người",
    "Đói cho sạch\nrách cho thơm",
    "Không ai giàu ba họ\nkhông ai khó ba đời",
    "Trăng mờ còn tỏ\nngười khó còn khôn",
    "Có chí thì nên",
    "Mưa dầm thấm lâu",
    "Học thầy\nkhông tày học bạn",
    "Giận quá mất khôn",
    "Khéo ăn thì no\nkhéo co thì ấm",
    "Điều lành\nđem lại điều hay",
    "Ghét của nào\ntrời trao của nấy",
    "Không có việc gì khó\nchỉ sợ lòng không bền",
    "Không làm sao nên",
    "Một điều nhịn\nchín điều lành"
};

const int quotes_count = 50;

char * get_current_quote(void) {
    uint32_t current_time = timer_read32();
    int quote_index = (((current_time / 1000 / 60 / 5) % quotes_count) + (quote_offset % quotes_count)) % quotes_count;
    return (char *)quotes[quote_index];
}
