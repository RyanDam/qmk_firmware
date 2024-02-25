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

#pragma once

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

// 10, 16, 19, 0, 1, 2

// SPI Configuration
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP18
#define SPI_MOSI_PIN GP19

// Display Configuration
#define OLED_CS_PIN GP0
#define OLED_DC_PIN GP16
#define OLED_RST_PIN GP1
#define OLED_BKL_PIN GP2

// #define BACKLIGHT_PWM_DRIVER PWMD0
// #define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
// #define BACKLIGHT_PIN GP2
// #define BACKLIGHT_LEVELS 5
// #define BACKLIGHT_DEFAULT_LEVEL 3

#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0
// #define QUANTUM_PAINTER_TASK_THROTTLE 999 // 50 // 1000 / 50 = 20 FPS
// #define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 4096

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 80
#define SCREEN_REFRESH_GAP_MS 41 // 1000 / 41 = 24 FPS

#define QP_LVGL_TASK_PERIOD 41

// #define ST7735_NO_AUTOMATIC_OFFSETS
