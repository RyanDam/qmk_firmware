#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE

#    include "graphics/ui.h"
#    include "hid/hid.h"
#    include "eeprom/cb_eeprom.h"
#    include "hardware/flash.h"

#ifdef DS1302_ENABLE
    #include "hardware/ds1302.h"
#endif // DS1302_ENABLE

#    define CB_SCREEN_NEXT QK_KB_0
#    define CB_SCREEN_PREV QK_KB_1
#    define CB_SCREEN_CLOCK QK_KB_2
#    define CB_SCREEN_ANIME QK_KB_3
#    define CB_SCREEN_LAYER QK_KB_4
#    define CB_SCREEN_POMO QK_KB_5
#    define CB_POMO_START QK_KB_6
#    define CB_POMO_CANCEL QK_KB_7

uint32_t             last_key_press_timestamp = 0;
bool                 layer_mode_activated     = true;
bool                 screen_boot_done         = false;
enum coban_screen_id last_screen_idx          = coban_screen_undefined;

uint8_t _current_keyboard_layer_idx = 0;

// key event task state
bool                 _key_event_detected = false;
enum coban_screen_id _target_screen_idx  = -1;
bool                 _need_save_eeprom   = false;
bool                 _skip_layer_change  = false;

void keyboard_post_init_user(void) {
    // Load eeprom
    coban_load_config();
    // Load gif data
    gif_data_header.data_size = EEPROM_MAX_GIF_SIZE;
    // gif_data_header.data_size = 29524;
    // load gif from flash to ram
    const uint8_t *pointer = (const uint8_t *)(XIP_BASE + EEROM_CB_GIF_ADDR);
    for (int i = 0; i < EEPROM_MAX_GIF_SIZE; i++) {
        gif_data[i] = *(pointer + i);
    }

#    ifdef DS1302_ENABLE
    // Initialize DS1302 RTC
    ds1302_init();
    ds1302_start_clock();

    // Read time from RTC and sync to clock screen
    ds1302_datetime_t rtc_time;
    if (ds1302_read_datetime(&rtc_time)) {
        screen_time_sync_datetime(rtc_time.hour, rtc_time.minute, rtc_time.second, rtc_time.day, rtc_time.month, rtc_time.year - 2000);
    }
#    endif

    // Init the display
    ui_init();
    last_key_press_timestamp = timer_read32();
}

void housekeeping_task_user(void) {
    // Draw the display
    // ui_task();

    uint32_t current_timestamp = timer_read32();
    if (current_timestamp < KEYBOAD_BOOT_TIME) {
        return;
    }

    if (screen_boot_done == false) {
        last_screen_idx  = change_screen(config.screen_idx);
        screen_boot_done = true;
    }

    uint32_t current_idle_time_ms = current_timestamp - last_key_press_timestamp;
    uint32_t idle_time_ms         = config.screen_idle_timeout * 60 * 1000;

    // handle backlight idling
    if (idle_time_ms > 0                                                                     // 0 means disable idle timeout
        && current_idle_time_ms > idle_time_ms && screen_pomodoro_session_running() == false // when pomodoro is running, dont turn screen off
    ) {
        if (is_backlight_enabled()) {
            backlight_disable();
        }
        // return;
    }

    // handle layer logic
    bool need_show_layer_change = false;
    if (config.layer_switch_default && current_idle_time_ms >= config.layer_switch_default_timeout * 1000 && _current_keyboard_layer_idx != 0 // default layer is 0
    ) {
        layer_clear(); // reset to default layer
        // show layer change
        // need_show_layer_change = true;
        // _skip_layer_change = false;
        // last_key_press_timestamp = timer_read32();
    }

    if (_key_event_detected || need_show_layer_change) {
        _key_event_detected = false;
        if (_target_screen_idx != current_screen()) {
            // if current screen is different
            // -> screen change request is detected
            // -> prioritized screen change
            change_screen(_target_screen_idx);
            if (layer_mode_activated) {
                // current screen state switched to layer
                // ensure we switch back to correct screen
                last_screen_idx      = _target_screen_idx;
                layer_mode_activated = false;
            }
        } else if (config.screen_switch_layer && layer_mode_activated == false && current_screen() != coban_screen_layer && !_skip_layer_change) {
            // Then check if layer switching is needed
            last_screen_idx = current_screen();
            change_screen(coban_screen_layer);
            layer_mode_activated = true;
        }

        if (_need_save_eeprom) {
            config.screen_idx = _target_screen_idx;
            coban_save_config();
        }
    }

    // handle screen layer switch back
    if (config.screen_switch_layer && current_idle_time_ms > config.screen_switch_layer_timeout * 1000 && layer_mode_activated == true) {
        change_screen(last_screen_idx);
        layer_mode_activated = false;
    }
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    backlight_disable();
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    backlight_enable();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint32_t current_timestamp = timer_read32();
    if (current_timestamp < KEYBOAD_BOOT_TIME) {
        return true;
    }
    _current_keyboard_layer_idx = get_highest_layer(state);
    screen_layers_set_indice(_current_keyboard_layer_idx);
    return state;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_key_press_timestamp = timer_read32();

    uint32_t current_timestamp = timer_read32();
    if (current_timestamp < KEYBOAD_BOOT_TIME) {
        return;
    }

    if (!is_backlight_enabled()) {
        backlight_enable();
    }

    enum coban_screen_id current_screen_idx = current_screen();
    // if (current_screen_idx == coban_screen_layer) {
    //     screen_layers_set_key_code(keycode, record);
    // }
    // screen_render_set_key_code(keycode, record);

    // reset state
    _target_screen_idx = current_screen_idx;
    _need_save_eeprom  = false;
    _skip_layer_change = false;

    if (record->event.pressed) {
        _key_event_detected = true;
        switch (keycode) {
            case CB_SCREEN_NEXT: {
                _target_screen_idx = next_screen(false);
                _need_save_eeprom  = true;
                break;
            }
            case CB_SCREEN_PREV: {
                _target_screen_idx = prev_screen(false);
                _need_save_eeprom  = true;
                break;
            }
            case CB_SCREEN_CLOCK: {
                _target_screen_idx = coban_screen_clock;
                _need_save_eeprom  = true;
                break;
            }
            case CB_SCREEN_ANIME: {
                _target_screen_idx = coban_screen_anime;
                _need_save_eeprom  = true;
                break;
            }
            case CB_SCREEN_LAYER: {
                _target_screen_idx = coban_screen_layer;
                _need_save_eeprom  = true;
                break;
            }
            case CB_SCREEN_POMO: {
                _target_screen_idx = coban_screen_pomodoro;
                _need_save_eeprom  = true;
                break;
            }
            case CB_POMO_START: {
                screen_pomodoro_session_start();
                _target_screen_idx = coban_screen_pomodoro;
                _skip_layer_change = true;
                break;
            }
            case CB_POMO_CANCEL: {
                screen_pomodoro_session_cancel();
                _target_screen_idx = coban_screen_pomodoro;
                _skip_layer_change = true;
                break;
            }
            default:
                break;
        }
    }

    return;
}

#endif // QUANTUM_PAINTER_ENABLE
