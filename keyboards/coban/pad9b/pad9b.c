#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE

#include "graphics/ui.h"
#include "hid/hid.h"
#include "eeprom/cb_eeprom.h"
#include "hardware/flash.h"

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

    // Load config
    change_screen(config.screen_idx);
    screen_time_set_format(
        config.time_style_id,
        config.time_format,
        config.time_indicator,
        config.date_format,
        config.date_visibility
    );

    // keyboard_post_init_user();
}

// void housekeeping_task_kb(void) {
//     // Draw the display
//     // ui_task();

// // #ifdef ENCODER_ENABLE
// //     encoder_read();
// // #endif
// }

// void suspend_power_down_user(void) {
//     // code will run multiple times while keyboard is suspended
//     backlight_disable();
// }

// void suspend_wakeup_init_user(void) {
//     // code will run on keyboard wakeup
//     backlight_enable();
// }

layer_state_t layer_state_set_user(layer_state_t state) {
    if (config.screen_idx == coban_screen_layer) {
        screen_layers_set_indice(get_highest_layer(state));
    }
    return state;
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//       case KC_ENTER:
//         return true; // Let QMK send the enter press/release events
//       default:
//         return true; // Process all other keycodes normally
//     }
// }

#endif //QUANTUM_PAINTER_ENABLE
