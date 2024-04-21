/* Copyright 2023 RyanDam (https://github.com/RyanDam)
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

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        TO(1),
        KC_MPRV,   KC_UP,   KC_MNXT,
        KC_LEFT, KC_DOWN, KC_RIGHT
    ),
    [1] = LAYOUT(
        TO(2),
        KC_1,   KC_2,   KC_3,
        KC_4,   KC_5,   KC_6
    ),
    [2] = LAYOUT(
        TO(3),
        KC_A,   KC_B,   KC_C,
        KC_D,   KC_E,   KC_F
    ),
    [3] = LAYOUT(
        TO(0),
        QK_BACKLIGHT_DOWN, QK_BACKLIGHT_UP,   QK_BOOT,
        KC_F1, KC_F2, KC_F3
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif
