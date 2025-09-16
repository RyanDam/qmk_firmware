#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE

#include "graphics/ui.h"
#include "hid/hid.h"
#include "eeprom/cb_eeprom.h"
#include "hardware/flash.h"

#define CB_SCREEN_NEXT      QK_KB_0
#define CB_SCREEN_PREV      QK_KB_1
#define CB_SCREEN_CLOCK     QK_KB_2
#define CB_SCREEN_ANIME     QK_KB_3
#define CB_SCREEN_LAYER     QK_KB_4
#define CB_SCREEN_POMO      QK_KB_5
#define CB_POMO_START       QK_KB_6
#define CB_POMO_CANCEL      QK_KB_7

// #define CB_SCREEN_STATS     QK_KB_3
// #define CB_SCREEN_RENDER    QK_KB_6

uint32_t last_key_press_timestamp = 0;
bool screen_turned_back = true;
bool screen_boot_done = false;

void keyboard_post_init_kb(void) {

    // Load eeprom
    coban_load_config();
    // Load gif data
    gif_data_header.data_size = EEPROM_MAX_GIF_SIZE;
    // gif_data_header.data_size = 29524;
    // load gif from flash to ram
    const uint8_t *pointer = (const uint8_t *) (XIP_BASE + EEROM_CB_GIF_ADDR);
    for (int i = 0; i < EEPROM_MAX_GIF_SIZE; i++) {
        gif_data[i] = *(pointer + i);
    }

    // Init the display
    ui_init();
    last_key_press_timestamp = timer_read32();

    // Load config
    // change_screen(config.screen_idx);
    // screen_time_set_format(
    //     config.time_style_id,
    //     config.time_format,
    //     config.time_indicator,
    //     config.date_format,
    //     config.date_visibility
    // );

    // keyboard_post_init_user();
}



void housekeeping_task_user(void) {
    // Draw the display
    // ui_task();

    uint32_t current_timestamp = timer_read32();
    if (current_timestamp < KEYBOAD_BOOT_TIME) {
        return;
    }

    if (screen_boot_done == false) {
        change_screen(config.screen_idx);
        screen_boot_done = true;
    }

    uint32_t current_idle_time_ms = current_timestamp - last_key_press_timestamp;
    uint32_t idle_time_ms = config.screen_idle_timeout * 60 * 1000;

    // handle backlight idling
    if (
        idle_time_ms > 0 // 0 means disable idle timeout
        && current_idle_time_ms > idle_time_ms
    ) {
        if (is_backlight_enabled()) {
            backlight_disable();
        }
        return;
    }

    // handle screen layer switch back
    if (current_idle_time_ms > config.screen_switch_layer_timeout*1000) {
        if (
            config.screen_switch_layer
            && config.screen_idx != coban_screen_layer
            && screen_turned_back == false
        ) {
            change_screen(config.screen_idx);
            screen_turned_back = true;
        }
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

    // if (config.screen_idx == coban_screen_layer) {
        screen_layers_set_indice(get_highest_layer(state));
    // }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    uint32_t current_timestamp = timer_read32();
    if (current_timestamp < KEYBOAD_BOOT_TIME) {
        return true;
    }

    last_key_press_timestamp = timer_read32();
    if (
        config.screen_switch_layer
        && config.screen_idx != coban_screen_layer
        && screen_turned_back == true
    ) {
        change_screen(coban_screen_layer);
        screen_turned_back = false;
    }

    if (!is_backlight_enabled()) {
        backlight_enable();
    }

    // if (config.screen_idx == coban_screen_layer) {
        // screen_layers_set_key_code(keycode, record);
    // }

    screen_render_set_key_code(keycode, record);

    if (record->event.pressed) {
        switch (keycode) {
            case CB_SCREEN_NEXT: {
                config.screen_idx = next_screen() & 0xff;
                coban_save_config();
                break;
            }
            case CB_SCREEN_PREV: {
                config.screen_idx = prev_screen() & 0xff;
                coban_save_config();
                break;
            }
            case CB_SCREEN_CLOCK: {
                config.screen_idx = coban_screen_clock;
                change_screen(config.screen_idx);
                coban_save_config();
                break;
            }
            // case CB_SCREEN_STATS: {
            //     config.screen_idx = coban_screen_stats;
            //     change_screen(config.screen_idx);
            //     coban_save_config();
            //     break;
            // }
            case CB_SCREEN_ANIME: {
                config.screen_idx = coban_screen_anime;
                change_screen(config.screen_idx);
                coban_save_config();
                break;
            }
            case CB_SCREEN_LAYER: {
                config.screen_idx = coban_screen_layer;
                change_screen(config.screen_idx);
                coban_save_config();
                break;
            }
            // case CB_SCREEN_RENDER: {
            //     config.screen_idx = coban_screen_render;
            //     change_screen(config.screen_idx);
            //     coban_save_config();
            //     break;
            // }
            case CB_SCREEN_POMO: {
                config.screen_idx = coban_screen_pomodoro;
                change_screen(config.screen_idx);
                coban_save_config();
            }
            case CB_POMO_START: {
                screen_pomodoro_session_start();
                change_screen(coban_screen_pomodoro);
                break;
            }
            case CB_POMO_CANCEL: {
                screen_pomodoro_session_cancel();
                change_screen(coban_screen_pomodoro);
                break;
            }
            default:
                break;
        }
    }

    return true;
}

#endif //QUANTUM_PAINTER_ENABLE
