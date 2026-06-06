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

const theme_color_t *get_theme_colors(void) {
    static theme_color_t theme;
    theme.background     = lv_color_make(config.theme_colors[0], config.theme_colors[1], config.theme_colors[2]);
    theme.text_primary   = lv_color_make(config.theme_colors[3], config.theme_colors[4], config.theme_colors[5]);
    theme.text_secondary = lv_color_make(config.theme_colors[6], config.theme_colors[7], config.theme_colors[8]);
    theme.accent         = lv_color_make(config.theme_colors[9], config.theme_colors[10], config.theme_colors[11]);
    theme.inactive       = lv_color_make(config.theme_colors[12], config.theme_colors[13], config.theme_colors[14]);
    return &theme;
}
