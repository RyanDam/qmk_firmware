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
#include "eeprom/cb_eeprom.h"
#include "graphics/ui.h"
#include "graphics/screens/styles.h"

static painter_device_t oled;

lv_obj_t *screen_clock = NULL;
// lv_obj_t *screen_stats = NULL;
lv_obj_t *screen_anime    = NULL;
lv_obj_t *screen_layer    = NULL;
lv_obj_t *screen_render   = NULL;
lv_obj_t *screen_pomodoro = NULL;
lv_obj_t *screen_boot     = NULL;

const int                   screen_indexes[]  = {coban_screen_clock, coban_screen_anime, coban_screen_layer, coban_screen_pomodoro};
const int                   num_avail_screen  = 4;
static enum coban_screen_id ui_current_screen = coban_screen_undefined;

static lv_timer_t *ui_timer          = NULL;
static bool        ui_timmer_running = false;

void ui_init(void) {
    oled = qp_st7735_make_spi_device(SCREEN_HEIGHT, SCREEN_WIDTH, OLED_CS_PIN, OLED_DC_PIN, OLED_RST_PIN, 1, 0);

    qp_init(oled, QP_ROTATION_270);

#ifdef OLED_PART_REV_2
    qp_set_viewport_offsets(oled, 1, 26);
#endif // OLED_PART_REV_2

    if (qp_lvgl_attach(oled)) {
        lv_disp_t  *lv_display = lv_disp_get_default();
        lv_theme_t *lv_theme   = lv_theme_default_init(lv_display, lv_palette_main(LV_PALETTE_AMBER), lv_palette_main(LV_PALETTE_BLUE), true, LV_FONT_DEFAULT);
        lv_disp_set_theme(lv_display, lv_theme);

        init_styles();

        screen_boot = screen_boot_init();
        lv_scr_load(screen_boot);

        // screen_stats = screen_hardware_stat_init();
        screen_clock = screen_time_init();
        screen_anime = screen_animation_init();
        screen_layer = screen_layers_init();
        // screen_render = screen_render_init();
        screen_pomodoro = screen_pomodoro_init();
    }

    backlight_enable();
    screen_ui_reload();
}

enum coban_screen_id current_screen(void) {
    return ui_current_screen;
}

enum coban_screen_id next_screen(bool need_change_screen) {
    int next_screen_idx = coban_screen_clock;
    for (int i = 0; i < num_avail_screen; i++) {
        if (ui_current_screen == screen_indexes[i]) {
            next_screen_idx = screen_indexes[(i + 1) % num_avail_screen];
            break;
        }
    }
    if (need_change_screen) {
        return change_screen(next_screen_idx & 0xff);
    }
    return next_screen_idx & 0xff;
}

enum coban_screen_id prev_screen(bool need_change_screen) {
    int prev_screen_idx = coban_screen_clock;
    for (int i = 0; i < num_avail_screen; i++) {
        if (ui_current_screen == screen_indexes[i]) {
            prev_screen_idx = screen_indexes[(i - 1 + num_avail_screen) % num_avail_screen];
            break;
        }
    }
    if (need_change_screen) {
        return change_screen(prev_screen_idx & 0xff);
    }
    return prev_screen_idx & 0xff;
}

enum coban_screen_id change_screen(uint8_t screen_idx) {
    // animate screen need self refresh when loading new gif buffer
    if (screen_idx == ui_current_screen && screen_idx != coban_screen_anime) {
        return ui_current_screen;
    }

    switch (screen_idx) {
        case coban_screen_clock: {
            screen_animation_stop();
            // screen_render_stop();
            screen_pomodoro_stop();
            screen_layers_stop();
            lv_scr_load(screen_clock);
            screen_time_reload();
            ui_current_screen = coban_screen_clock;
            break;
        }
        // case coban_screen_stats: {
        //     screen_animation_stop();
        //     // screen_render_stop();
        //     screen_time_stop();
        //     screen_pomodoro_stop();
        //     screen_layers_stop();
        //     lv_scr_load(screen_stats);
        //     ui_current_screen = coban_screen_stats;
        //     break;
        // }
        case coban_screen_anime: {
            // screen_render_stop();
            screen_time_stop();
            screen_pomodoro_stop();
            screen_layers_stop();
            lv_scr_load(screen_anime);
            screen_animation_reload();
            ui_current_screen = coban_screen_anime;
            break;
        }
        case coban_screen_layer: {
            screen_animation_stop();
            // screen_render_stop();
            screen_time_stop();
            screen_pomodoro_stop();
            lv_scr_load(screen_layer);
            screen_layers_reload();
            ui_current_screen = coban_screen_layer;
            break;
        }
        // case coban_screen_render: {
        //     screen_animation_stop();
        //     screen_time_stop();
        //     screen_pomodoro_stop();
        //     screen_layers_stop();
        //     lv_scr_load(screen_render);
        //     screen_render_reload();
        //     ui_current_screen = coban_screen_render;
        //     break;
        // }
        case coban_screen_pomodoro: {
            screen_animation_stop();
            // screen_render_stop();
            screen_time_stop();
            screen_layers_stop();
            lv_scr_load(screen_pomodoro);
            screen_pomodoro_reload();
            ui_current_screen = coban_screen_pomodoro;
            break;
        }
        default:
            break;
    }

    // if (screen_boot != NULL) {
    //     // save memory after the boot sequence is complete
    //     lv_obj_del(screen_boot);
    // }

    return ui_current_screen;
}

static void ui_cb(lv_timer_t *timer) {
    if (!ui_timmer_running) {
        return;
    }

    if (timer_running) {
        timer_cb(timer);
    }

    // if (render_running) {
    //     render_cb(timer);
    // }

    if (pomo_running) {
        pomo_cb(timer);
    }

    if (layers_running) {
        layers_cb(timer);
    }
}

void screen_ui_stop(void) {
    if (!ui_timmer_running) {
        return;
    }
    ui_timmer_running = false;
    lv_timer_pause(ui_timer);
}

void screen_ui_reload(void) {
    if (ui_timmer_running) {
        return;
    }
    ui_timmer_running = true;
    if (ui_timer == NULL) {
        ui_timer = lv_timer_create(ui_cb, 100, NULL);
    } else {
        lv_timer_resume(ui_timer);
    }
}

#ifdef OLED_PART_REV_2
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
#endif // OLED_PART_REV_2
