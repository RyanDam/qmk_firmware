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
static theme_color_t theme_gray;
static theme_color_t theme_pink;
static theme_color_t theme_olive;
static theme_color_t theme_orange;

static const theme_color_t *themes[coban_theme_count] = {
    &theme_dark, &theme_light, &theme_gray, &theme_pink, &theme_olive, &theme_orange,
};

static void init_theme_colors(void) {
    theme_dark.bg                   = lv_color_hex(0x000000);
    theme_dark.text_primary         = lv_color_hex(0xFFFFFF);
    theme_dark.text_secondary       = lv_color_hex(0xFFFFFF);
    theme_dark.accent               = lv_color_hex(0x0FFFFFF);
    theme_dark.surface              = lv_color_hex(0xFFFFFF);
    theme_dark.surface_pressed      = lv_color_hex(0xFFFFFF);
    theme_dark.surface_pressed_text = lv_color_hex(0x000000);
    theme_dark.inactive             = lv_color_hex(0xFFFFFF);
    theme_dark.pomo_bg              = lv_color_hex(0x252525);
    theme_dark.pomo_indicator       = lv_color_hex(0x009664);

    theme_light.bg                   = lv_color_hex(0xFFFFFF);
    theme_light.text_primary         = lv_color_hex(0x000000);
    theme_light.text_secondary       = lv_color_hex(0x000000);
    theme_light.accent               = lv_color_hex(0x000000);
    theme_light.surface              = lv_color_hex(0x000000);
    theme_light.surface_pressed      = lv_color_hex(0x000000);
    theme_light.surface_pressed_text = lv_color_hex(0xFFFFFF);
    theme_light.inactive             = lv_color_hex(0x000000);
    theme_light.pomo_bg              = lv_color_hex(0x252525);
    theme_light.pomo_indicator       = lv_color_hex(0x009664);

    theme_gray.bg                   = lv_color_hex(0x0A2727);
    theme_gray.text_primary         = lv_color_hex(0xCECECE);
    theme_gray.text_secondary       = lv_color_hex(0xCECECE);
    theme_gray.accent               = lv_color_hex(0x969696);
    theme_gray.surface              = lv_color_hex(0x969696);
    theme_gray.surface_pressed      = lv_color_hex(0x707070);
    theme_gray.surface_pressed_text = lv_color_hex(0xFFFFFF);
    theme_gray.inactive             = lv_color_hex(0xCECECE);
    theme_gray.pomo_bg              = lv_color_hex(0x383838);
    theme_gray.pomo_indicator       = lv_color_hex(0x969696);

    theme_pink.bg                   = lv_color_hex(0x8F2946);
    theme_pink.text_primary         = lv_color_hex(0xD08F93);
    theme_pink.text_secondary       = lv_color_hex(0xD08F93);
    theme_pink.accent               = lv_color_hex(0xFFB2B3);
    theme_pink.surface              = lv_color_hex(0xFFB2B3);
    theme_pink.surface_pressed      = lv_color_hex(0xE09090);
    theme_pink.surface_pressed_text = lv_color_hex(0xFFFFFF);
    theme_pink.inactive             = lv_color_hex(0xD08F93);
    theme_pink.pomo_bg              = lv_color_hex(0x382028);
    theme_pink.pomo_indicator       = lv_color_hex(0xFFB2B3);

    theme_olive.bg                   = lv_color_hex(0x0E251D);
    theme_olive.text_primary         = lv_color_hex(0x56D783);
    theme_olive.text_secondary       = lv_color_hex(0x56D783);
    theme_olive.accent               = lv_color_hex(0x384011);
    theme_olive.surface              = lv_color_hex(0x5A6A20);
    theme_olive.surface_pressed      = lv_color_hex(0x384011);
    theme_olive.surface_pressed_text = lv_color_hex(0xE8E8D0);
    theme_olive.inactive             = lv_color_hex(0x56D783);
    theme_olive.pomo_bg              = lv_color_hex(0x202810);
    theme_olive.pomo_indicator       = lv_color_hex(0x169A44);

    theme_orange.bg                   = lv_color_hex(0x502014);
    theme_orange.text_primary         = lv_color_hex(0xFF894E);
    theme_orange.text_secondary       = lv_color_hex(0xFF894E);
    theme_orange.accent               = lv_color_hex(0x943605);
    theme_orange.surface              = lv_color_hex(0xC05010);
    theme_orange.surface_pressed      = lv_color_hex(0x943605);
    theme_orange.surface_pressed_text = lv_color_hex(0xFFF0E0);
    theme_orange.inactive             = lv_color_hex(0xFF894E);
    theme_orange.pomo_bg              = lv_color_hex(0x382018);
    theme_orange.pomo_indicator       = lv_color_hex(0x943605);
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
