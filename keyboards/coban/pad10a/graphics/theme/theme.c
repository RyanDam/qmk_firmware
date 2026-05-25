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

#include "graphics/theme/theme.h"
#include "eeprom/cb_eeprom.h"

static theme_color_t theme_dark;
static theme_color_t theme_light;

static const theme_color_t *themes[coban_theme_count] = {
    &theme_dark,
    &theme_light,
};

static void init_theme_colors(void) {
    theme_dark.bg                   = lv_color_hex(0x000000);
    theme_dark.text_primary         = lv_color_hex(0xFFFFFF);
    theme_dark.text_secondary       = lv_color_hex(0xFFFFFF);
    theme_dark.accent               = lv_color_hex(0x0096FF);
    theme_dark.surface              = lv_color_hex(0xFFFFFF);
    theme_dark.surface_pressed      = lv_color_hex(0xFFFFFF);
    theme_dark.surface_pressed_text = lv_color_hex(0x000000);
    theme_dark.inactive             = lv_color_hex(0xFFFFFF);
    theme_dark.pomo_bg              = lv_color_hex(0x252525);
    theme_dark.pomo_indicator       = lv_color_hex(0x009664);

    theme_light.bg                   = lv_color_hex(0xFFFFFF);
    theme_light.text_primary         = lv_color_hex(0x000000);
    theme_light.text_secondary       = lv_color_hex(0x000000);
    theme_light.accent               = lv_color_hex(0x0066FF);
    theme_light.surface              = lv_color_hex(0x000000);
    theme_light.surface_pressed      = lv_color_hex(0x000000);
    theme_light.surface_pressed_text = lv_color_hex(0xFFFFFF);
    theme_light.inactive             = lv_color_hex(0x000000);
    theme_light.pomo_bg              = lv_color_hex(0x252525);
    theme_light.pomo_indicator       = lv_color_hex(0x009664);
}

const theme_color_t *get_current_theme(void) {
    static bool initialized = false;
    if (!initialized) {
        init_theme_colors();
        initialized = true;
    }
    uint8_t id = config.theme_id;
    if (id >= coban_theme_count) {
        id = coban_theme_dark;
    }
    return themes[id];
}
