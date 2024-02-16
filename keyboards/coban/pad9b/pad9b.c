#include "quantum.h"
// #include "raw_hid.h"

// #ifdef RAW_ENABLE

// void raw_hid_receive(uint8_t *data, uint8_t length) {
//     uint8_t response[length];
//     memset(response, 0, length);
//     response[0] = 'B';

//     if(data[0] == 'A') {
//         raw_hid_send(response, length);
//     }
// }

// #endif // RAW_ENABLE

#ifdef QUANTUM_PAINTER_ENABLE

#include "graphics/ui.h"

void keyboard_post_init_kb(void) {
    // Init the display
    ui_init();
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    // Draw the display
    ui_task();
}

#endif //QUANTUM_PAINTER_ENABLE
