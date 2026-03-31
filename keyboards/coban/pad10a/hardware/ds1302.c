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

#include "quantum.h"
#include "wait.h"

#ifdef DS1302_ENABLE

#    include "hardware/ds1302.h"

bool ds1302_initialized = false;

// DS1302 timing requirements (from datasheet)
#    define DS1302_CLK_DELAY_US 30

// // BCD to decimal conversion
// #define DS1302_BCD_TO_DEC(bcd) (((bcd) >> 4) * 10 + ((bcd) & 0x0F))
// // Decimal to BCD conversion
// #define DS1302_DEC_TO_BCD(dec) (((dec) / 10) << 4 | ((dec) % 10))

// Private function prototypes
static void    ds1302_delay_us(uint8_t us);
static void    ds1302_write_byte(uint8_t byte);
static uint8_t ds1302_read_byte(void);
static void    ds1302_start_tx(void);
static void    ds1302_end_tx(void);

static void ds1302_delay_us(uint8_t us) {
    wait_us(DS1302_CLK_DELAY_US);
    // uint32_t start = timer_read32();
    // while (timer_elapsed32(start) < us) {
    //     // Wait for microseconds
    // }
}

bool ds1302_init(void) {
    // Set pins as output where needed
    gpio_set_pin_output(DS1302_PIN_RESET);
    gpio_set_pin_output(DS1302_PIN_SCK);

    // Set initial states
    gpio_write_pin_low(DS1302_PIN_RESET);
    gpio_write_pin_low(DS1302_PIN_SCK);

    // Write 0 to write protect register to enable writes
    ds1302_start_tx();
    ds1302_write_byte(0x8e);
    ds1302_write_byte(0x00);
    ds1302_end_tx();

    // Write 0 to control register
    ds1302_start_tx();
    ds1302_write_byte(0x90);
    ds1302_write_byte(0x00);
    ds1302_end_tx();

    ds1302_initialized = true;

    return true;
}

static void ds1302_start_tx(void) {
    gpio_write_pin_low(DS1302_PIN_SCK);
    gpio_write_pin_high(DS1302_PIN_RESET);
}

static void ds1302_end_tx(void) {
    gpio_write_pin_low(DS1302_PIN_SCK);
    gpio_write_pin_low(DS1302_PIN_RESET);
}

static void ds1302_write_byte(uint8_t byte) {
    gpio_set_pin_output(DS1302_PIN_SDA);
    for (uint8_t i = 0; i < 8; i++) {
        bool bit = (byte >> i) & 0x01;
        gpio_write_pin_low(DS1302_PIN_SCK);
        ds1302_delay_us(DS1302_CLK_DELAY_US);
        if (bit) {
            gpio_write_pin_high(DS1302_PIN_SDA);
        } else {
            gpio_write_pin_low(DS1302_PIN_SDA);
        }
        gpio_write_pin_high(DS1302_PIN_SCK);
        ds1302_delay_us(DS1302_CLK_DELAY_US);
    }
}

static uint8_t ds1302_read_byte(void) {
    gpio_set_pin_input(DS1302_PIN_SDA);
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        gpio_write_pin_high(DS1302_PIN_SCK);
        ds1302_delay_us(DS1302_CLK_DELAY_US);
        gpio_write_pin_low(DS1302_PIN_SCK);
        ds1302_delay_us(DS1302_CLK_DELAY_US);
        bool bit = gpio_read_pin(DS1302_PIN_SDA);
        byte |= (bit << i);
    }
    return byte;
}

bool ds1302_read_datetime(ds1302_datetime_t *datetime) {
    if (datetime == NULL) {
        return false;
    }

    // Use burst mode for reading: 0xbf = read from second register with auto-increment
    ds1302_start_tx();
    ds1302_write_byte(0xbf);
    uint8_t data[7] = {};
    for (int i = 0; i < 7; i++) {
        data[i] = ds1302_read_byte();
    }
    ds1302_end_tx();

    // Read 7 bytes (second, minute, hour, weekday, day, month, year)
    datetime->second  = ((data[0] & 0x70) >> 4) * 10 + (data[0] & 0x0f);
    datetime->minute  = ((data[1] & 0x70) >> 4) * 10 + (data[1] & 0x0f);
    datetime->hour    = ((data[2] & 0x30) >> 4) * 10 + (data[2] & 0x0f);
    datetime->day     = ((data[3] & 0x30) >> 4) * 10 + (data[3] & 0x0f);
    datetime->month   = ((data[4] & 0x10) >> 4) * 10 + (data[4] & 0x0f);
    datetime->weekday = ((data[5] & 0x30) >> 4) * 10 + (data[3] & 0x0f);
    datetime->year    = ((data[6] & 0xf0) >> 4) * 10 + (data[6] & 0x0f);

    return true;
}

bool ds1302_write_datetime(const ds1302_datetime_t *datetime) {
    if (datetime == NULL) {
        return false;
    }

    uint8_t data[9] = {};
    data[0] = (((uint8_t)(datetime->second  / 10)) << 4) | (datetime->second % 10); // DS1302_DEC_TO_BCD(datetime->second);
    data[1] = (((uint8_t)(datetime->minute  / 10)) << 4) | (datetime->minute % 10); // DS1302_DEC_TO_BCD(datetime->minute);
    data[2] = (((uint8_t)(datetime->hour    / 10)) << 4) | (datetime->hour % 10); // DS1302_DEC_TO_BCD(datetime->hour);
    data[3] = (((uint8_t)(datetime->day     / 10)) << 4) | (datetime->day % 10); // DS1302_DEC_TO_BCD(datetime->day);
    data[4] = (((uint8_t)(datetime->month   / 10)) << 4) | (datetime->month % 10); // DS1302_DEC_TO_BCD(datetime->month);
    data[5] = (((uint8_t)(datetime->weekday / 10)) << 4) | (datetime->weekday % 10); // DS1302_DEC_TO_BCD(datetime->weekday);
    data[6] = (((uint8_t)(datetime->year    / 10)) << 4) | (datetime->year % 10); // DS1302_DEC_TO_BCD(datetime->year);
    data[7] = 0;
    data[8] = 0;

    // Use burst mode for writing: 0xbe = write to second register with auto-increment
    ds1302_start_tx();
    ds1302_write_byte(0xbe);
    for (int i = 0; i < 9; i++) {
        ds1302_write_byte(data[i]);
    }
    ds1302_end_tx();

    return true;
}

#endif // DS1302_ENABLE
