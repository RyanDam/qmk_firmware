#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE

#include "graphics/ui.h"

void keyboard_post_init_kb(void) {
    // Init the display
    ui_init();
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    // Draw the display
    // ui_task();

// #ifdef ENCODER_ENABLE
//     encoder_read();
// #endif
}

#endif //QUANTUM_PAINTER_ENABLE
