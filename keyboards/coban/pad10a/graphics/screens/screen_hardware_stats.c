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

#include "graphics/screens/screen_hardware_stats.h"
#include "graphics/lvgl_helpers.h"
#include "graphics/screens/styles.h"
#include "eeprom/cb_eeprom.h"
#include <string.h>

#define STAT_BAR_COL_PAD 6
#define STATS_SLOT_COUNT 4
#define STAT_ALL_PAD 4

static lv_obj_t          *screen_home                 = NULL;
static lv_obj_t          *mods                        = NULL;
static lv_obj_t          *layout_2x1_holder           = NULL;
static lv_obj_t          *layout_2x2_holder           = NULL;
static lv_obj_t          *layout_hybrid_holder        = NULL;
static coban_stats_data_t stat_data[STATS_DATA_COUNT] = {{0}};
static uint8_t            current_layout              = coban_stats_layout_2x1;

typedef struct {
    lv_obj_t *label;
    lv_obj_t *value;
    lv_obj_t *arc;
} arc_ui_t;

typedef struct {
    lv_obj_t *label;
    lv_obj_t *value;
    lv_obj_t *bar;
} bar_ui_t;

static arc_ui_t arc_ui[2]                = {{NULL}};
static bar_ui_t bar_ui[STATS_SLOT_COUNT] = {{NULL}};

static const char *get_data_name(uint8_t data_id) {
    switch (data_id) {
        case coban_stats_data_cpu_util:
            return "CPU";
        case coban_stats_data_cpu_temp:
            return "CPU";
        case coban_stats_data_ram:
            return "RAM";
        case coban_stats_data_gpu_util:
            return "GPU";
        case coban_stats_data_gpu_temp:
            return "GPU";
        case coban_stats_data_disk_usage:
            return "DISK";
        case coban_stats_data_disk_total:
            return "DISK";
        case coban_stats_data_net_sent:
            return "NET↓";
        case coban_stats_data_net_recv:
            return "NET↑";
        case coban_stats_data_swap_usage:
            return "SWAP";
        case coban_stats_data_cpu_freq:
            return "CPU";
        case coban_stats_data_soc_temp:
            return "SOC";
        case coban_stats_data_disk_read:
            return "D.R";
        case coban_stats_data_disk_write:
            return "D.W";
        case coban_stats_data_cpu_power:
            return "CPU";
        case coban_stats_data_gpu_power:
            return "GPU";
        default:
            return "";
    }
}

static const char *get_unit_symbol(uint8_t unit_id) {
    switch (unit_id) {
        case coban_stats_unit_percent:
            return "%";
        case coban_stats_unit_celsius:
            return "°C";
        case coban_stats_unit_fahrenheit:
            return "°F";
        case coban_stats_unit_megabytes:
            return "MB";
        case coban_stats_unit_gigabytes:
            return "GB";
        case coban_stats_unit_terabytes:
            return "TB";
        case coban_stats_unit_mbps:
            return "Mbps";
        case coban_stats_unit_mhz:
            return "MHz";
        case coban_stats_unit_watts:
            return "W";
        default:
            return "";
    }
}

static bool needs_decimal_format(uint8_t data_id) {
    switch (data_id) {
        case coban_stats_data_ram:
        case coban_stats_data_disk_usage:
        case coban_stats_data_disk_total:
        case coban_stats_data_swap_usage:
        case coban_stats_data_net_sent:
        case coban_stats_data_net_recv:
        case coban_stats_data_disk_read:
        case coban_stats_data_disk_write:
            return true;
        default:
            return false;
    }
}

static lv_obj_t *create_arc_ui(uint8_t index, lv_obj_t *parent, uint8_t data_id) {
    lv_obj_t *holder = lv_obj_create(parent);
    lv_obj_add_style(holder, &style_container, 0);
    use_flex_column(holder);
    lv_obj_set_style_pad_row(holder, -12, 0);

    lv_obj_t *indicator_holder = lv_obj_create(holder);
    lv_obj_add_style(indicator_holder, &style_container, 0);

    arc_ui[index].arc = lv_arc_create(indicator_holder);
    lv_obj_set_size(arc_ui[index].arc, 64, 64);
    lv_obj_add_style(arc_ui[index].arc, &style_arc_main, LV_PART_MAIN);
    lv_obj_add_style(arc_ui[index].arc, &style_arc_positive, LV_PART_INDICATOR);
    lv_obj_remove_style(arc_ui[index].arc, NULL, LV_PART_KNOB);
    lv_arc_set_rotation(arc_ui[index].arc, 135);
    lv_arc_set_bg_angles(arc_ui[index].arc, 0, 270);
    lv_arc_set_value(arc_ui[index].arc, 0);
    lv_obj_center(arc_ui[index].arc);

    arc_ui[index].value = lv_label_create(indicator_holder);
    lv_label_set_text(arc_ui[index].value, "--");
    lv_obj_add_style(arc_ui[index].value, &style_text, 0);
    lv_obj_center(arc_ui[index].value);

    arc_ui[index].label = lv_label_create(holder);
    lv_label_set_text(arc_ui[index].label, get_data_name(data_id));
    lv_obj_add_style(arc_ui[index].label, &style_text, 0);
    lv_obj_center(arc_ui[index].label);

    return holder;
}

static lv_obj_t *create_bar_ui(uint8_t index, lv_obj_t *parent, uint8_t data_id) {
    lv_obj_t *holder = lv_obj_create(parent);
    lv_obj_add_style(holder, &style_container, 0);
    use_flex_column(holder);
    lv_obj_set_style_pad_all(holder, 0, 0);
    lv_obj_set_size(holder, SCREEN_WIDTH / 2 - STAT_BAR_COL_PAD / 2 - STAT_ALL_PAD, SCREEN_HEIGHT / 2 - STAT_ALL_PAD);

    lv_obj_t *text_row = lv_obj_create(holder);
    lv_obj_add_style(text_row, &style_container, 0);
    use_flex_row(text_row);
    lv_obj_set_flex_align(text_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(text_row, LV_PCT(100), SCREEN_HEIGHT / 4);

    bar_ui[index].label = lv_label_create(text_row);
    lv_label_set_text(bar_ui[index].label, get_data_name(data_id));
    lv_obj_add_style(bar_ui[index].label, &style_text, 0);

    bar_ui[index].value = lv_label_create(text_row);
    lv_label_set_text(bar_ui[index].value, "--");
    lv_obj_add_style(bar_ui[index].value, &style_text, 0);

    lv_obj_t *bar_row = lv_obj_create(holder);
    lv_obj_add_style(bar_row, &style_container, 0);
    use_flex_row(bar_row);
    lv_obj_set_flex_align(bar_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(bar_row, LV_PCT(100), SCREEN_HEIGHT / 4 - STAT_ALL_PAD);

    bar_ui[index].bar = lv_bar_create(bar_row);
    lv_obj_set_size(bar_ui[index].bar, LV_PCT(100), SCREEN_HEIGHT / 4 - STAT_ALL_PAD);
    lv_obj_add_style(bar_ui[index].bar, &style_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar_ui[index].bar, &style_bar_positive, LV_PART_INDICATOR);
    lv_bar_set_range(bar_ui[index].bar, 0, 100);
    lv_bar_set_value(bar_ui[index].bar, 0, LV_ANIM_OFF);

    return holder;
}

static void update_arc_display(uint8_t index, uint8_t data_index) {
    if (arc_ui[index].label == NULL || arc_ui[index].arc == NULL || arc_ui[index].value == NULL) {
        return;
    }

    const char *unit_symbol = get_unit_symbol(stat_data[data_index].unit_id);
    char        buf[16];

    if (stat_data[data_index].max_value > 0) {
        uint8_t percentage = (uint8_t)((stat_data[data_index].value * 100UL) / stat_data[data_index].max_value);
        lv_arc_set_value(arc_ui[index].arc, (int16_t)percentage);
        if (needs_decimal_format(data_index)) {
            uint16_t left  = stat_data[data_index].value / 10;
            uint16_t right = stat_data[data_index].value % 10;
            snprintf(buf, sizeof(buf), "%u.%01u%s", left, right, unit_symbol);
        } else {
            snprintf(buf, sizeof(buf), "%u%s", stat_data[data_index].value, unit_symbol);
        }
        lv_label_set_text(arc_ui[index].value, buf);
        lv_label_set_text_fmt(arc_ui[index].label, "%s", get_data_name(data_index));
    } else {
        lv_arc_set_value(arc_ui[index].arc, 0);
        snprintf(buf, sizeof(buf), "--%s", unit_symbol);
        lv_label_set_text(arc_ui[index].value, buf);
        lv_label_set_text_fmt(arc_ui[index].label, "%s", get_data_name(data_index));
    }
}

static void update_bar_display(uint8_t index, uint8_t data_index) {
    if (bar_ui[index].label == NULL || bar_ui[index].value == NULL || bar_ui[index].bar == NULL) {
        return;
    }

    const char *unit_symbol = get_unit_symbol(stat_data[data_index].unit_id);
    char        buf[16];

    if (stat_data[data_index].max_value > 0) {
        uint8_t percentage = (uint8_t)((stat_data[data_index].value * 100UL) / stat_data[data_index].max_value);
        lv_bar_set_value(bar_ui[index].bar, percentage, LV_ANIM_OFF);
        if (needs_decimal_format(data_index)) {
            uint16_t left  = stat_data[data_index].value / 10;
            uint16_t right = stat_data[data_index].value % 10;
            snprintf(buf, sizeof(buf), "%u.%01u%s", left, right, unit_symbol);
        } else {
            snprintf(buf, sizeof(buf), "%u%s", stat_data[data_index].value, unit_symbol);
        }
        lv_label_set_text(bar_ui[index].value, buf);
        lv_label_set_text_fmt(bar_ui[index].label, "%s", get_data_name(data_index));
    } else {
        lv_bar_set_value(bar_ui[index].bar, 0, LV_ANIM_OFF);
        snprintf(buf, sizeof(buf), "--%s", unit_symbol);
        lv_label_set_text(bar_ui[index].value, buf);
        lv_label_set_text_fmt(bar_ui[index].label, "%s", get_data_name(data_index));
    }
}

static void build_layout_2x1(void) {
    if (layout_2x1_holder == NULL) {
        layout_2x1_holder = lv_obj_create(mods);
        lv_obj_add_style(layout_2x1_holder, &style_container, 0);
        use_flex_row(layout_2x1_holder);
        lv_obj_set_flex_align(layout_2x1_holder, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

        for (uint8_t i = 0; i < 2; i++) {
            uint8_t data_id = config.stats_data_ids[i];
            create_arc_ui(i, layout_2x1_holder, data_id);
        }
    }
}

static void build_layout_2x2(void) {
    if (layout_2x2_holder == NULL) {
        layout_2x2_holder = lv_obj_create(mods);
        lv_obj_add_style(layout_2x2_holder, &style_container, 0);
        use_flex_column(layout_2x2_holder);
        lv_obj_set_style_pad_all(layout_2x2_holder, 0, 0);
        lv_obj_set_size(layout_2x2_holder, SCREEN_WIDTH, SCREEN_HEIGHT);

        lv_obj_t *row1 = lv_obj_create(layout_2x2_holder);
        lv_obj_add_style(row1, &style_container, 0);
        use_flex_row(row1);
        lv_obj_set_flex_align(row1, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_all(row1, 0, 0);
        lv_obj_set_style_pad_column(row1, STAT_BAR_COL_PAD, 0);

        lv_obj_t *row2 = lv_obj_create(layout_2x2_holder);
        lv_obj_add_style(row2, &style_container, 0);
        use_flex_row(row2);
        lv_obj_set_flex_align(row2, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_all(row2, 0, 0);
        lv_obj_set_style_pad_column(row2, STAT_BAR_COL_PAD, 0);

        for (uint8_t i = 0; i < 2; i++) {
            uint8_t data_id = config.stats_data_ids[i];
            create_bar_ui(i, row1, data_id);
        }
        for (uint8_t i = 2; i < STATS_SLOT_COUNT; i++) {
            uint8_t data_id = config.stats_data_ids[i];
            create_bar_ui(i, row2, data_id);
        }
    }
}

static void build_layout_hybrid(void) {
    if (layout_hybrid_holder == NULL) {
        layout_hybrid_holder = lv_obj_create(mods);
        lv_obj_add_style(layout_hybrid_holder, &style_container, 0);
        use_flex_row(layout_hybrid_holder);
        lv_obj_set_flex_align(layout_hybrid_holder, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_all(layout_hybrid_holder, STAT_ALL_PAD, 0);
        lv_obj_set_style_pad_row(layout_hybrid_holder, 0, 0);
        lv_obj_set_style_pad_column(layout_hybrid_holder, 0, 0);
        lv_obj_set_size(layout_hybrid_holder, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Left side: single arc (data index 0)
        lv_obj_t *left_col = lv_obj_create(layout_hybrid_holder);
        lv_obj_add_style(left_col, &style_container, 0);
        use_flex_column(left_col);
        lv_obj_set_flex_align(left_col, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_all(left_col, 0, 0);
        lv_obj_set_size(left_col, SCREEN_WIDTH / 2 + STAT_BAR_COL_PAD / 2 - STAT_ALL_PAD, LV_PCT(100));

        uint8_t data_id = config.stats_data_ids[0];
        create_arc_ui(0, left_col, data_id);

        // Right side: 2 bars stacked vertically (data index 1 and 2)
        lv_obj_t *right_col = lv_obj_create(layout_hybrid_holder);
        lv_obj_add_style(right_col, &style_container, 0);
        use_flex_column(right_col);
        lv_obj_set_flex_align(right_col, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_all(right_col, 0, 0);
        lv_obj_set_style_pad_row(right_col, 0, 0);
        lv_obj_set_style_pad_column(right_col, 0, 0);
        lv_obj_set_size(right_col, SCREEN_WIDTH / 2 - STAT_BAR_COL_PAD / 2 - STAT_ALL_PAD, LV_PCT(100));

        for (uint8_t i = 0; i < 2; i++) {
            uint8_t bar_data_id = config.stats_data_ids[1 + i];
            create_bar_ui(i, right_col, bar_data_id);
        }
    }
}

static void update_all_displays(void) {
    if (current_layout == coban_stats_layout_2x1) {
        for (uint8_t i = 0; i < 2; i++) {
            uint8_t data_id = config.stats_data_ids[i];
            if (data_id != coban_stats_data_none && data_id < STATS_DATA_COUNT) {
                update_arc_display(i, data_id);
            }
        }
    } else if (current_layout == coban_stats_layout_2x2) {
        for (uint8_t i = 0; i < STATS_SLOT_COUNT; i++) {
            uint8_t data_id = config.stats_data_ids[i];
            if (data_id != coban_stats_data_none && data_id < STATS_DATA_COUNT) {
                update_bar_display(i, data_id);
            }
        }
    } else if (current_layout == coban_stats_layout_hybrid) {
        {
            uint8_t data_id = config.stats_data_ids[0];
            if (data_id != coban_stats_data_none && data_id < STATS_DATA_COUNT) {
                update_arc_display(0, data_id);
            }
        }
        for (uint8_t i = 0; i < 2; i++) {
            uint8_t data_id = config.stats_data_ids[1 + i];
            if (data_id != coban_stats_data_none && data_id < STATS_DATA_COUNT) {
                update_bar_display(i, data_id);
            }
        }
    }
}

lv_obj_t *screen_hardware_stat_init(void) {
    if (screen_home == NULL) {
        screen_home = lv_obj_create(NULL);
        lv_obj_add_style(screen_home, &style_screen, 0);
        use_flex_column(screen_home);

        mods = lv_obj_create(screen_home);
        lv_obj_add_style(mods, &style_container, 0);
        use_flex_column(mods);

        build_layout_2x1();
        build_layout_2x2();
        build_layout_hybrid();
    }

    current_layout = config.stats_layout_id;

    if (layout_2x1_holder != NULL) {
        if (current_layout != coban_stats_layout_2x1) {
            lv_obj_add_flag(layout_2x1_holder, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(layout_2x1_holder, LV_OBJ_FLAG_HIDDEN);
        }
    }
    if (layout_2x2_holder != NULL) {
        if (current_layout != coban_stats_layout_2x2) {
            lv_obj_add_flag(layout_2x2_holder, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(layout_2x2_holder, LV_OBJ_FLAG_HIDDEN);
        }
    }
    if (layout_hybrid_holder != NULL) {
        if (current_layout != coban_stats_layout_hybrid) {
            lv_obj_add_flag(layout_hybrid_holder, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(layout_hybrid_holder, LV_OBJ_FLAG_HIDDEN);
        }
    }

    update_all_displays();

    return screen_home;
}

void screen_hardware_stat_set_data(uint8_t data_id, uint16_t value, uint16_t max_value, uint8_t unit_id) {
    if (data_id >= STATS_DATA_COUNT) {
        return;
    }

    stat_data[data_id].value     = value;
    stat_data[data_id].max_value = max_value;
    stat_data[data_id].unit_id   = unit_id;

    update_all_displays();
}

void screen_hardware_stat_set_layout(uint8_t layout_id) {
    if (layout_id != current_layout) {
        current_layout = layout_id;
        if (layout_2x1_holder != NULL) {
            if (current_layout != coban_stats_layout_2x1) {
                lv_obj_add_flag(layout_2x1_holder, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_clear_flag(layout_2x1_holder, LV_OBJ_FLAG_HIDDEN);
            }
        }
        if (layout_2x2_holder != NULL) {
            if (current_layout != coban_stats_layout_2x2) {
                lv_obj_add_flag(layout_2x2_holder, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_clear_flag(layout_2x2_holder, LV_OBJ_FLAG_HIDDEN);
            }
        }
        if (layout_hybrid_holder != NULL) {
            if (current_layout != coban_stats_layout_hybrid) {
                lv_obj_add_flag(layout_hybrid_holder, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_clear_flag(layout_hybrid_holder, LV_OBJ_FLAG_HIDDEN);
            }
        }
        update_all_displays();
    }
}

void screen_hardware_stat_reload(void) {
    if (layout_2x1_holder != NULL) {
        lv_obj_del(layout_2x1_holder);
        layout_2x1_holder = NULL;
    }
    if (layout_2x2_holder != NULL) {
        lv_obj_del(layout_2x2_holder);
        layout_2x2_holder = NULL;
    }
    if (layout_hybrid_holder != NULL) {
        lv_obj_del(layout_hybrid_holder);
        layout_hybrid_holder = NULL;
    }
    memset(arc_ui, 0, sizeof(arc_ui));
    memset(bar_ui, 0, sizeof(bar_ui));
    screen_hardware_stat_init();
}
