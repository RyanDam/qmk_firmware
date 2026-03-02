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

static lv_obj_t *screen_layers      = NULL;
bool             layers_running     = false;
static bool      key_matrix_changed = false;
static bool      key_layer_changed  = false;

static lv_obj_t *holder_layer_indices;

static lv_obj_t *indices[4];
static lv_obj_t *keys[MATRIX_ROWS * MATRIX_COLS];
static uint16_t  key_codes[MATRIX_ROWS * MATRIX_COLS];
static bool      key_presses[MATRIX_ROWS * MATRIX_COLS];
static uint8_t   current_layer_idx;

void _update_layer_keycode(uint8_t layer_idx);

lv_obj_t *screen_layers_init(void) {
    screen_layers = lv_obj_create(NULL);
    lv_obj_add_style(screen_layers, &style_screen, 0);
    lv_obj_set_style_pad_all(screen_layers, 0, 0);
    // use_flex_column(screen_layers);
    // lv_obj_set_flex_align(screen_layers, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *layers_holder = lv_obj_create(screen_layers);
    lv_obj_add_style(layers_holder, &style_container, 0);
    use_flex_column(layers_holder);
    lv_obj_set_style_pad_row(layers_holder, 0, 0);
    lv_obj_set_style_pad_column(layers_holder, 0, 0);
    lv_obj_set_width(layers_holder, lv_pct(100));
    lv_obj_set_height(layers_holder, lv_pct(100));

    for (int r = 0; r < MATRIX_ROWS; r++) {
        lv_obj_t *holder_row = lv_obj_create(layers_holder);
        lv_obj_add_style(holder_row, &style_container, 0);
        use_flex_row(holder_row);
        lv_obj_set_style_pad_column(holder_row, 0, 0);
        lv_obj_set_width(holder_row, lv_pct(100));
        lv_obj_set_flex_align(holder_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        for (int c = 0; c < MATRIX_COLS; c++) {
            int idx   = r * MATRIX_COLS + c;
            keys[idx] = lv_label_create(holder_row);
            char key_label[8];
            snprintf(key_label, sizeof(key_label), "key%d", idx);
            lv_label_set_text(keys[idx], key_label);
            if (idx == 1 || idx == 2) {
                lv_obj_add_style(keys[idx], &style_key_trans, 0);
            } else {
                lv_obj_add_style(keys[idx], &style_key, 0);
            }

        }
    }

    holder_layer_indices = lv_obj_create(screen_layers);
    lv_obj_add_style(holder_layer_indices, &style_container, 0);
    lv_obj_set_style_pad_column(holder_layer_indices, 8, 0);
    use_flex_row(holder_layer_indices);
    lv_obj_set_style_pad_left(holder_layer_indices, 12, 0);
    lv_obj_set_style_pad_right(holder_layer_indices, 12, 0);
    lv_obj_align(holder_layer_indices, LV_ALIGN_TOP_MID, 0, 8);

    for (int l = 0; l < 4; l++) {
        indices[l] = lv_obj_create(holder_layer_indices);
        lv_obj_add_style(indices[l], &style_indice, 0);
    }

    current_layer_idx = 0;
    for (int idx = 0; idx < MATRIX_ROWS * MATRIX_COLS; idx++) {
        key_presses[idx] = false;
    }
    screen_layers_set_indice(current_layer_idx);

    return screen_layers;
}

void _update_layer_keycode(uint8_t layer_idx) {
    for (int idx = 0; idx < MATRIX_ROWS * MATRIX_COLS; idx++) {
        if (idx == 1 || idx == 2) {
            lv_obj_add_style(keys[idx], &style_key_trans, 0);
        } else {
            lv_obj_add_style(keys[idx], &style_key, 0);
        }
        // lv_obj_add_style(keys[idx], &style_key, 0);
    }
    for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            uint16_t keycode = dynamic_keymap_get_keycode(layer_idx, r, c);
            lv_label_set_text_fmt(keys[r * MATRIX_COLS + c], translate_keycode_to_string(keycode));
            key_codes[r * MATRIX_COLS + c] = keycode;
        }
    }
}

void screen_layers_set_indice(uint8_t layer_idx) {
    current_layer_idx = layer_idx;
    _update_layer_keycode(layer_idx);
    key_layer_changed = true;
}

void screen_layers_set_key_code(uint16_t keycode, keyrecord_t *record) {
    for (int idx = 0; idx < MATRIX_ROWS * MATRIX_COLS; idx++) {
        if (key_codes[idx] != keycode) continue;
        key_presses[idx] = record->event.pressed;
    }
    key_matrix_changed = true;
}

void layers_cb(lv_timer_t *timer) {
    if (!layers_running) return;

    if (key_layer_changed) {
        key_layer_changed = false;
        for (uint8_t l = 0; l < 4; l++) {
            if (l == current_layer_idx) {
                lv_obj_set_style_bg_opa(indices[l], LV_OPA_100, 0);
            } else {
                lv_obj_set_style_bg_opa(indices[l], LV_OPA_0, 0);
            }
        }
    }

    if (key_matrix_changed) {
        key_matrix_changed = false;
        for (int idx = 0; idx < MATRIX_ROWS * MATRIX_COLS; idx++) {
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
