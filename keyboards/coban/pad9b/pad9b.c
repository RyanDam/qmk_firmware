#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE

#include "graphics/ui.h"
#include "hid/hid.h"
#include "eeprom/cb_eeprom.h"

void keyboard_post_init_kb(void) {

    // Load eeprom
    coban_load_config();
    // Load gif data
    gif_data_header.data_size = config.gif_data_size;
    eeprom_read_block(&gif_data, (void *)EEROM_CB_GIF_ADDR, config.gif_data_size);

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

    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    // Draw the display
    // ui_task();

// #ifdef ENCODER_ENABLE
//     encoder_read();
// #endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
    screen_layers_set_indice(get_highest_layer(state));
    return state;
}

#endif //QUANTUM_PAINTER_ENABLE
