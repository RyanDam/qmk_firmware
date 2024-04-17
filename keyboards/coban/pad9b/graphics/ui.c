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

#include "qp.h"
#include "qp_st7735.h"
#include "backlight/backlight.h"

#include "graphics/ui.h"
// #include "graphics/engine/render_bangle.h"
#include "graphics/screens/styles.h"

static painter_device_t oled;

lv_obj_t * screen_clock;
lv_obj_t * screen_stats;
lv_obj_t * screen_anime;
lv_obj_t * screen_layer;

void ui_init(void) {

    oled = qp_st7735_make_spi_device(SCREEN_HEIGHT, SCREEN_WIDTH, OLED_CS_PIN, OLED_DC_PIN, OLED_RST_PIN, 2, 0);

    qp_init(oled, QP_ROTATION_270);
    qp_set_viewport_offsets(oled, 1, 26);

    if (qp_lvgl_attach(oled)) {
        lv_disp_t  *lv_display = lv_disp_get_default();
        lv_theme_t *lv_theme   = lv_theme_default_init(lv_display, lv_palette_main(LV_PALETTE_AMBER), lv_palette_main(LV_PALETTE_BLUE), true, LV_FONT_DEFAULT);
        lv_disp_set_theme(lv_display, lv_theme);

        init_styles();

        screen_stats = screen_hardware_stat_init();
        screen_clock = screen_time_init();
        screen_anime = screen_animation_init();
        screen_layer = screen_layers_init();

        // change_screen(coban_screen_clock);
    }

    // render_bangle_init(oled);

    backlight_enable();
}

void change_screen(uint8_t screen_idx) {
    switch (screen_idx) {
        case coban_screen_clock: {
            lv_scr_load(screen_clock);
            break;
        }
        case coban_screen_stats: {
            lv_scr_load(screen_stats);
            break;
        }
        case coban_screen_anime: {
            lv_scr_load(screen_anime);
            break;
        }
        case coban_screen_layer: {
            lv_scr_load(screen_layer);
            break;
        }
        default:
            break;
    }
}

void ui_task(void) {

    // static uint32_t last_draw = 0;
    // uint32_t        now       = timer_read32();
    // if (TIMER_DIFF_32(now, last_draw) < SCREEN_REFRESH_GAP_MS) // Throttle to 30fps
    //     return;

    // // clean screan
    // qp_rect(oled, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, true);

    // render_bangle_task(oled);

    // uint32_t last       = timer_read32();
    // // uint32_t total_draw = TIMER_DIFF_32(last, now);
    // // sprintf(c, "%ld ms", total_draw);
    // // qp_drawtext(oled, 5, 5, font, c);

    // last_draw = last;
}
