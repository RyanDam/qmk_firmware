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

#include "quantum/dynamic_keymap.h"
#include "graphics/screens/screen_layers.h"
#include "graphics/screens/styles.h"
#include "graphics/lvgl_helpers.h"
#include "utils/keycode_lookup.h"

static lv_obj_t *screen_layers = NULL;
bool layers_running = false;
static bool key_matrix_changed = false;
static bool key_layer_changed = false;

static lv_obj_t *holder_layer1;
static lv_obj_t *holder_layer2;
static lv_obj_t *holder_layer3;
static lv_obj_t *holder_layer_indices;

// static lv_obj_t *indice_0;
// static lv_obj_t *indice_1;
// static lv_obj_t *indice_2;
// static lv_obj_t *indice_3;

static lv_obj_t *indices[4];
static lv_obj_t *keys[MATRIX_ROWS*MATRIX_COLS];
static uint16_t key_codes[MATRIX_ROWS*MATRIX_COLS];
static bool key_presses[MATRIX_ROWS*MATRIX_COLS];
static uint8_t current_layer_idx;

void _update_layer_keycode(uint8_t layer_idx);

lv_obj_t * screen_layers_init(void) {
    screen_layers = lv_obj_create(NULL);
    lv_obj_add_style(screen_layers, &style_screen, 0);
    use_flex_column(screen_layers);
    lv_obj_set_style_pad_row(screen_layers, 0, 0);
    lv_obj_set_style_pad_column(screen_layers, 0, 0);

    ///////////////
    //// Row 1 ////
    ///////////////
    holder_layer1 = lv_obj_create(screen_layers);
    lv_obj_add_style(holder_layer1, &style_container, 0);
    use_flex_row(holder_layer1);
    lv_obj_set_style_pad_column(holder_layer1, 0, 0);
    lv_obj_set_width(holder_layer1, lv_pct(100));
    lv_obj_set_flex_align(holder_layer1, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    keys[0] = lv_label_create(holder_layer1);
    lv_label_set_text(keys[0], "key0");
    lv_obj_add_style(keys[0], &style_key, 0);
    // lv_obj_set_width(key_0, lv_pct(33));

    holder_layer_indices = lv_obj_create(holder_layer1);
    lv_obj_add_style(holder_layer_indices, &style_container, 0);
    lv_obj_set_style_pad_column(holder_layer_indices, 8, 0);
    use_flex_row(holder_layer_indices);
    lv_obj_set_style_pad_left(holder_layer_indices, 12, 0);
    lv_obj_set_style_pad_right(holder_layer_indices, 12, 0);
    // lv_obj_set_width(holder_layer_indices, lv_pct(66));

    indices[0] = lv_obj_create(holder_layer_indices);
    lv_obj_add_style(indices[0], &style_indice, 0);

    indices[1] = lv_obj_create(holder_layer_indices);
    lv_obj_add_style(indices[1], &style_indice, 0);

    indices[2] = lv_obj_create(holder_layer_indices);
    lv_obj_add_style(indices[2], &style_indice, 0);

    indices[3] = lv_obj_create(holder_layer_indices);
    lv_obj_add_style(indices[3], &style_indice, 0);

    ///////////////
    //// Row 2 ////
    ///////////////
    holder_layer2 = lv_obj_create(screen_layers);
    lv_obj_add_style(holder_layer2, &style_container, 0);
    use_flex_row(holder_layer2);
    lv_obj_set_style_pad_column(holder_layer2, 0, 0);
    lv_obj_set_width(holder_layer2, lv_pct(100));
    lv_obj_set_flex_align(holder_layer2, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    keys[1] = lv_label_create(holder_layer2);
    lv_label_set_text(keys[1] , "key1");
    lv_obj_add_style(keys[1] , &style_key, 0);
    // lv_obj_set_width(key_1, lv_pct(33));

    keys[2] = lv_label_create(holder_layer2);
    lv_label_set_text(keys[2], "key2");
    lv_obj_add_style(keys[2], &style_key, 0);
    // lv_obj_set_width(key_2, lv_pct(33));

    keys[3] = lv_label_create(holder_layer2);
    lv_label_set_text(keys[3], "key3");
    lv_obj_add_style(keys[3], &style_key, 0);
    // lv_obj_set_width(key_3, lv_pct(33));

    ///////////////
    //// Row 3 ////
    ///////////////
    holder_layer3 = lv_obj_create(screen_layers);
    lv_obj_add_style(holder_layer3, &style_container, 0);
    use_flex_row(holder_layer3);
    lv_obj_set_style_pad_column(holder_layer3, 0, 0);
    lv_obj_set_width(holder_layer3, lv_pct(100));
    lv_obj_set_flex_align(holder_layer3, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    keys[4] = lv_label_create(holder_layer3);
    lv_label_set_text(keys[4], "key4");
    lv_obj_add_style(keys[4], &style_key, 0);
    // lv_obj_set_width(key_4, lv_pct(33));

    keys[5] = lv_label_create(holder_layer3);
    lv_label_set_text(keys[5], "key5");
    lv_obj_add_style(keys[5], &style_key, 0);
    // lv_obj_set_width(key_5, lv_pct(33));

    keys[6] = lv_label_create(holder_layer3);
    lv_label_set_text(keys[6], "key6");
    lv_obj_add_style(keys[6], &style_key, 0);
    // lv_obj_set_width(key_6, lv_pct(33));


    current_layer_idx = 0;
    for (int idx=0; idx < MATRIX_ROWS*MATRIX_COLS; idx++) {
        key_presses[idx] = false;
    }
    screen_layers_set_indice(current_layer_idx);


    return screen_layers;
}

void _update_layer_keycode(uint8_t layer_idx) {
    for (int idx=0; idx < MATRIX_ROWS*MATRIX_COLS; idx++) {
        lv_obj_add_style(keys[idx], &style_key, 0);
    }
    for (int r=0; r<MATRIX_ROWS; r++) {
        for (int c=0; c<MATRIX_COLS; c++) {
            uint16_t keycode = dynamic_keymap_get_keycode(layer_idx, r, c);
            // lv_label_set_text_fmt(keys[r*MATRIX_COLS + c], "%d", keycode);
            lv_label_set_text_fmt(keys[r*MATRIX_COLS + c], translate_keycode_to_string(keycode));
            key_codes[r*MATRIX_COLS + c] = keycode;
        }
    }
}

void screen_layers_set_indice(uint8_t layer_idx) {
    current_layer_idx = layer_idx;
    _update_layer_keycode(layer_idx);
    key_layer_changed = true;
}

void screen_layers_set_key_code(uint16_t keycode, keyrecord_t *record) {
    for (int idx=0; idx < MATRIX_ROWS*MATRIX_COLS; idx++) {
        if (key_codes[idx] != keycode) continue;
        key_presses[idx] = record->event.pressed;
    }
    key_matrix_changed = true;
}

void layers_cb(lv_timer_t * timer) {
    if (!layers_running) return;

    if (key_layer_changed) {
        key_layer_changed = false;
        // update layer indicator
        for (uint8_t l=0; l<4; l++) {
            if (l == current_layer_idx) {
                lv_obj_set_style_bg_opa(indices[l], LV_OPA_100, 0);
            } else {
                lv_obj_set_style_bg_opa(indices[l], LV_OPA_0, 0);
            }
        }
    }

    if (key_matrix_changed) {
        key_matrix_changed = false;
        // update key press state
        for (int idx=0; idx < MATRIX_ROWS*MATRIX_COLS; idx++) {
            if (key_presses[idx]) {
                lv_obj_add_style(keys[idx], &style_key_pressed, 0);
            } else {
                lv_obj_add_style(keys[idx], &style_key, 0);
            }
        }
    }
}

void screen_layers_stop(void) {
    if (screen_layers == NULL) return;
    if (!layers_running) {
        return;
    }
    layers_running = false;
}

void screen_layers_reload(void) {
    if (screen_layers == NULL) return;
    if (layers_running) {
        return;
    }
    layers_running = true;
}
