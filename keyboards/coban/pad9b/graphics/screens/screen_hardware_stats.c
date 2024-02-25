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

static lv_obj_t *screen_home;
static lv_obj_t *cpu_arc;
static lv_obj_t *cpu_text;
static lv_obj_t *gpu_arc;
static lv_obj_t *gpu_text;

lv_obj_t * screen_hardware_stat_init(void) {
    screen_home = lv_obj_create(NULL);
    lv_obj_add_style(screen_home, &style_screen, 0);
    use_flex_column(screen_home);

    lv_obj_t *mods = lv_obj_create(screen_home);
    lv_obj_add_style(mods, &style_container, 0);
    use_flex_column(mods);

    // label_render_time = lv_label_create(mods);
    // lv_label_set_text(label_render_time, "COBAN STATIONERY");
    // lv_obj_add_style(label_render_time, &style_text, 0);

    lv_obj_t *stats_holder = lv_obj_create(mods);
    lv_obj_add_style(stats_holder, &style_container, 0);
    use_flex_row(stats_holder);

    // CPU
    lv_obj_t *cpu_stat_holder = lv_obj_create(stats_holder);
    lv_obj_add_style(cpu_stat_holder, &style_container, 0);
    use_flex_column(cpu_stat_holder);

    lv_obj_t *cpu_arc_holder = lv_obj_create(cpu_stat_holder);
    lv_obj_add_style(cpu_arc_holder, &style_container, 0);

    cpu_arc = lv_arc_create(cpu_arc_holder);
    lv_obj_set_size(cpu_arc, 56, 56);
    lv_obj_add_style(cpu_arc, &style_arc_main, LV_PART_MAIN);
    lv_obj_add_style(cpu_arc, &style_arc_positive, LV_PART_INDICATOR);
    lv_obj_remove_style(cpu_arc, NULL, LV_PART_KNOB);
    lv_arc_set_rotation(cpu_arc, 135);
    lv_arc_set_bg_angles(cpu_arc, 0, 270);
    lv_arc_set_value(cpu_arc, 45);
    lv_obj_center(cpu_arc);

    cpu_text  = lv_label_create(cpu_arc_holder);
    lv_label_set_text(cpu_text, "24C");
    lv_obj_add_style(cpu_text, &style_text, 0);
    lv_obj_center(cpu_text);
    resize_font(cpu_text, 16);

    lv_obj_t *cpu_name  = lv_label_create(cpu_stat_holder);
    lv_label_set_text(cpu_name, "CPU");
    lv_obj_add_style(cpu_name, &style_text, 0);

    // GPU
    lv_obj_t *gpu_stat_holder = lv_obj_create(stats_holder);
    lv_obj_add_style(gpu_stat_holder, &style_container, 0);
    use_flex_column(gpu_stat_holder);

    lv_obj_t *gpu_arc_holder = lv_obj_create(gpu_stat_holder);
    lv_obj_add_style(gpu_arc_holder, &style_container, 0);

    gpu_arc = lv_arc_create(gpu_arc_holder);
    lv_obj_set_size(gpu_arc, 56, 56);
    lv_obj_add_style(gpu_arc, &style_arc_main, LV_PART_MAIN);
    lv_obj_add_style(gpu_arc, &style_arc_positive, LV_PART_INDICATOR);
    lv_obj_remove_style(gpu_arc, NULL, LV_PART_KNOB);
    lv_arc_set_rotation(gpu_arc, 135);
    lv_arc_set_bg_angles(gpu_arc, 0, 270);
    lv_arc_set_value(gpu_arc, 45);
    lv_obj_center(gpu_arc);

    gpu_text = lv_label_create(gpu_arc_holder);
    lv_label_set_text(gpu_text, "89%");
    lv_obj_add_style(gpu_text, &style_text, 0);
    lv_obj_center(gpu_text);
    resize_font(gpu_text, 16);

    lv_obj_t *gpu_name  = lv_label_create(gpu_stat_holder);
    lv_label_set_text(gpu_name, "GPU");
    lv_obj_add_style(gpu_name, &style_text, 0);

    lv_scr_load(screen_home);
    return screen_home;
}

bool screen_hardware_stat_is_visible(void) {
    return true;
}

void screen_hardware_stat_set_visible(void) {

}

void screen_hardware_stat_set_invisible(void) {

}

void screen_hardware_stat_set_cpu(uint8_t value) {
    lv_label_set_text_fmt(cpu_text, "%dC", value);
    lv_arc_set_value(cpu_arc, (int16_t)value);
}

void screen_hardware_stat_set_gpu(uint8_t value) {
    lv_label_set_text_fmt(gpu_text, "%d%%", value);
    lv_arc_set_value(gpu_arc, (int16_t)value);
}
