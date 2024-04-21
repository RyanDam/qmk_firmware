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
#include "qp_comms.h"
#include "qp_st7735.h"
#include "qp_st77xx_opcodes.h"
#include "qp_st7735_opcodes.h"

#include "backlight/backlight.h"

#include "graphics/ui.h"
#include "graphics/screens/styles.h"

static painter_device_t oled;

lv_obj_t * screen_clock;
lv_obj_t * screen_stats;
lv_obj_t * screen_anime;
lv_obj_t * screen_layer;

// inverse color for st7735
bool qp_st7735_init(painter_device_t device, painter_rotation_t rotation) {
    // clang-format off
    const uint8_t st7735_init_sequence[] = {
        // Command,                 Delay, N, Data[N]
        ST77XX_CMD_RESET,            120,  0,
        ST77XX_CMD_SLEEP_OFF,          5,  0,
        ST77XX_SET_PIX_FMT,            0,  1, 0x55,
        // ST77XX_CMD_INVERT_OFF,         0,  0,
        ST77XX_CMD_INVERT_ON,         0,  0,
        ST77XX_CMD_NORMAL_ON,          0,  0,
        ST7735_SET_PGAMMA,             0, 16, 0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10,
        ST7735_SET_NGAMMA,             0, 16, 0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,
        ST77XX_CMD_DISPLAY_ON,        20,  0
    };
    // clang-format on
    qp_comms_bulk_command_sequence(device, st7735_init_sequence, sizeof(st7735_init_sequence));

    // Configure the rotation (i.e. the ordering and direction of memory writes in GRAM)
    const uint8_t madctl[] = {
        [QP_ROTATION_0]   = ST77XX_MADCTL_BGR,
        [QP_ROTATION_90]  = ST77XX_MADCTL_BGR | ST77XX_MADCTL_MX | ST77XX_MADCTL_MV,
        [QP_ROTATION_180] = ST77XX_MADCTL_BGR | ST77XX_MADCTL_MX | ST77XX_MADCTL_MY,
        [QP_ROTATION_270] = ST77XX_MADCTL_BGR | ST77XX_MADCTL_MV | ST77XX_MADCTL_MY,
    };
    qp_comms_command_databyte(device, ST77XX_SET_MADCTL, madctl[rotation]);

// #ifndef ST7735_NO_AUTOMATIC_VIEWPORT_OFFSETS
//     st7735_automatic_viewport_offsets(device, rotation);
// #endif // ST7735_NO_AUTOMATIC_VIEWPORT_OFFSETS

    return true;
}

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
