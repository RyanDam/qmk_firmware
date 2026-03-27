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

#ifdef DS1302_ENABLE

#    include "hardware/ds1302.h"

bool ds1302_initialized = false;

// Private function prototypes
static void    ds1302_clock_toggle(void);
static void    ds1302_write_bit(bool value);
static bool    ds1302_read_bit(void);
static void    ds1302_write_byte(uint8_t byte);
static uint8_t ds1302_read_byte(void);
static void    ds1302_start_tx(void);
static void    ds1302_end_tx(void);

bool ds1302_init(void) {
    // Set pins as output where needed
    gpio_set_pin_output(DS1302_PIN_RESET);
    gpio_set_pin_output(DS1302_PIN_SCK);

    // Set initial states
    gpio_write_pin_low(DS1302_PIN_RESET);
    gpio_write_pin_low(DS1302_PIN_SCK);

    // Write 0 to write protect register to enable writes
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_WP | DS1302_WRITE_BIT);
    ds1302_write_byte(0x00);
    ds1302_end_tx();

    // Write 0 to control register
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_CTRL | DS1302_WRITE_BIT);
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

static void ds1302_clock_toggle(void) {
    gpio_write_pin_high(DS1302_PIN_SCK);
    gpio_write_pin_low(DS1302_PIN_SCK);
}

static void ds1302_write_bit(bool value) {
    if (value) {
        gpio_write_pin_high(DS1302_PIN_SDA);
    } else {
        gpio_write_pin_low(DS1302_PIN_SDA);
    }
    ds1302_clock_toggle();
}

static bool ds1302_read_bit(void) {
    gpio_write_pin_high(DS1302_PIN_SCK);
    bool value = gpio_read_pin(DS1302_PIN_SDA);
    gpio_write_pin_low(DS1302_PIN_SCK);
    return value;
}

static void ds1302_write_byte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        ds1302_write_bit((byte >> i) & 0x01);
    }
}

static uint8_t ds1302_read_byte(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (ds1302_read_bit()) {
            byte |= (1 << i);
        }
    }
    return byte;
}

bool ds1302_read_datetime(ds1302_datetime_t *datetime) {
    if (datetime == NULL) {
        return false;
    }

    // Use burst mode for reading: 0xbf = read from second register with auto-increment
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_SECOND | DS1302_READ_BIT | DS1302_MULTI_BIT);

    // Read 7 bytes (second, minute, hour, weekday, day, month, year)
    uint8_t second_reg = ds1302_read_byte();
    datetime->second   = DS1302_BCD_TO_DEC(second_reg & 0x7F);

    datetime->minute  = DS1302_BCD_TO_DEC(ds1302_read_byte());
    datetime->hour    = DS1302_BCD_TO_DEC(ds1302_read_byte());
    datetime->weekday = DS1302_BCD_TO_DEC(ds1302_read_byte());
    datetime->day     = DS1302_BCD_TO_DEC(ds1302_read_byte());
    datetime->month   = DS1302_BCD_TO_DEC(ds1302_read_byte());
    datetime->year    = DS1302_BCD_TO_DEC(ds1302_read_byte()) + 2000;

    ds1302_end_tx();

    return true;
}

bool ds1302_write_datetime(const ds1302_datetime_t *datetime) {
    if (datetime == NULL) {
        return false;
    }

    // Use burst mode for writing: 0xbe = write to second register with auto-increment
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_SECOND | DS1302_WRITE_BIT | DS1302_MULTI_BIT);

    uint8_t second_reg = DS1302_DEC_TO_BCD(datetime->second);
    ds1302_write_byte(second_reg);

    ds1302_write_byte(DS1302_DEC_TO_BCD(datetime->minute));
    ds1302_write_byte(DS1302_DEC_TO_BCD(datetime->hour));
    ds1302_write_byte(DS1302_DEC_TO_BCD(datetime->day));
    ds1302_write_byte(DS1302_DEC_TO_BCD(datetime->month));
    ds1302_write_byte(0x00); // Skip weekday

    ds1302_write_byte(DS1302_DEC_TO_BCD(datetime->year - 2000));

    ds1302_end_tx();

    return true;
}

bool ds1302_is_clock_running(void) {
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_SECOND | DS1302_READ_BIT | DS1302_MULTI_BIT);
    uint8_t second_reg = ds1302_read_byte();
    ds1302_end_tx();

    return !(second_reg & 0x80);
}

void ds1302_start_clock(void) {
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_SECOND | DS1302_WRITE_BIT);

    uint8_t second_reg = ds1302_read_byte();
    second_reg &= ~0x80; // Clear halt bit

    ds1302_write_byte(second_reg);
    ds1302_end_tx();
}

void ds1302_stop_clock(void) {
    ds1302_start_tx();
    ds1302_write_byte(DS1302_REG_SECOND | DS1302_WRITE_BIT);

    uint8_t second_reg = ds1302_read_byte();
    second_reg |= 0x80; // Set halt bit

    ds1302_write_byte(second_reg);
    ds1302_end_tx();
}

#endif // DS1302_ENABLE
