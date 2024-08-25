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

// SPI Configuration
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP2
#define SPI_MOSI_PIN GP3
#define SPI_MISO_PIN GP8

// Display Configuration
#define OLED_CS_PIN GP5
#define OLED_RST_PIN GP4
#define OLED_DC_PIN GP1
#define OLED_BKL_PIN GP0
#define OLED_DC_PIN GP1
#define OLED_BKL_PIN GP0

// Test platform config
// #define OLED_CS_PIN GP6
// #define OLED_RST_PIN GP4
// #define OLED_DC_PIN GP5
// #define OLED_BKL_PIN GP7

#ifdef BACKLIGHT_ENABLE
#   define BACKLIGHT_PWM_DRIVER PWMD0
#   define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#endif

#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0
// #define QUANTUM_PAINTER_TASK_THROTTLE 999 // 50 // 1000 / 50 = 20 FPS
// #define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 4096

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 80
#define SCREEN_REFRESH_GAP_MS 41 // 1000 / 41 = 24 FPS

#define QP_LVGL_TASK_PERIOD 41

#ifndef VIA_ENABLE
#   define VIA_EEPROM_CUSTOM_CONFIG_ADDR EECONFIG_SIZE
#endif

#define EEPROM_CB_CONFIG_SIZE 16
#define EEPROM_CB_CONFIG_ADDR (VIA_EEPROM_CUSTOM_CONFIG_ADDR)
#define VIA_EEPROM_CUSTOM_CONFIG_SIZE EEPROM_CB_CONFIG_SIZE

// https://www.makermatrix.com/blog/read-and-write-data-with-the-pi-pico-onboard-flash/
#define EEPROM_MAX_GIF_SIZE 102400 // 25*FLASH_SECTOR_SIZE // 50*4096 = 102400 B
#define EEROM_CB_GIF_ADDR (PICO_FLASH_SIZE_BYTES - EEPROM_MAX_GIF_SIZE - 4096) // TODO: find where eeprom addr end
