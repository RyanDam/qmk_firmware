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

#include "graphics/screens/screen_render.h"
#include "graphics/screens/styles.h"
#include "graphics/lvgl_helpers.h"

#include "graphics/engine/engine.h"
#include "graphics/engine/animation.h"
#include "graphics/resources/bangle.c"

static lv_obj_t *screen_render = NULL;
static lv_timer_t *render_timer = NULL;
static bool render_running = false;

/* Render resource */
static Matrix projectionMatrix;
static Matrix cameraMatrix;
static Vector cam_pos, cam_at, cam_up;
static uint32_t last_time_tick = 0;

/* Camera parameter - polar coordinate */
static const float cam_radius = 5.0f;
static float cam_radian = 0;
static const float cam_moving_speed = 10.0f; // degree per sec

/* Object modifier */
static AnimationValue object_height_scale;

/* Canvas buffer */
#define CANVAS_RENDER_WIDTH SCREEN_WIDTH
#define CANVAS_RENDER_HEIGHT SCREEN_HEIGHT
static lv_color_t canvas_render_buf[LV_IMG_BUF_SIZE_INDEXED_2BIT(CANVAS_RENDER_WIDTH, CANVAS_RENDER_HEIGHT)];
static lv_obj_t * canvas_render;
static lv_color_t color_render_background;
static lv_color_t color_render_midground;
static lv_color_t color_render_foreground;

/* Debug */
// static lv_obj_t *debug_text;

void screen_resource_init(void);

lv_obj_t * screen_render_init(void) {
    screen_render = lv_obj_create(NULL);
    lv_obj_add_style(screen_render, &style_screen, 0);

    canvas_render = lv_canvas_create(screen_render);  // attach to active screen
    lv_canvas_set_buffer(canvas_render, canvas_render_buf, CANVAS_RENDER_WIDTH, CANVAS_RENDER_HEIGHT, LV_IMG_CF_INDEXED_2BIT);
    lv_canvas_set_palette(canvas_render, 0, lv_color_hex(0x000000));
    lv_canvas_set_palette(canvas_render, 1, lv_color_hex(0xaaaaaa));
    lv_canvas_set_palette(canvas_render, 2, lv_color_hex(0xffffff));
    lv_obj_center(canvas_render); // place in middle

    color_render_background.full = 0;
    color_render_midground.full = 1;
    color_render_foreground.full = 2;
    lv_canvas_fill_bg(canvas_render, color_render_background, LV_OPA_COVER);

    screen_resource_init();

    // // debuging
    // debug_text = lv_label_create(screen_render);
    // lv_label_set_text(debug_text, "--");
    // lv_obj_add_style(debug_text, &style_text, 0);

    return screen_render;
}

void screen_resource_init(void) {
    // init camera matrix
    vector_fill4(&cam_pos, 0, 5, 5, 0);
    vector_fill4(&cam_at, 0, 0, 0, 0);
    vector_fill4(&cam_up, 0, 0, 1, 0);
    build_camera_matrix(&cam_pos, &cam_at, &cam_up, &cameraMatrix);

    // init projection matrix
    build_projection_matrix(CANVAS_RENDER_WIDTH, CANVAS_RENDER_HEIGHT, 1, 2, 100, &projectionMatrix);

    // init animation
    animation_init_set(&object_height_scale, 2.0f, 0.5f, 2.0f, 0.5f, 0.5f, timer_read32());
}

static void anim_cb(lv_timer_t * timer) {
    if (!render_running) {
        return;
    }
    // clear screen
    lv_canvas_fill_bg(canvas_render, color_render_background, LV_OPA_COVER);

    uint32_t delta_time = timer_read32() - last_time_tick;
    last_time_tick = timer_read32(); // update time tick

    //////////////////////
    // Animation update //
    //////////////////////

    animation_update(&object_height_scale, timer_read32());
    // return back to original
    animation_set_target(&object_height_scale, object_height_scale.target - object_height_scale.delta_speed);

    ///////////////////////
    // Projection update //
    ///////////////////////

    // the camera should rotate cam_moving_speed degree per sec
    // delta_time is in ms, so we need to convert it to seconds
    cam_radian += cam_moving_speed * M_PI/180 * delta_time / 1000;
    cam_radian -= ((int)(cam_radian/(2*M_PI))) * (2*M_PI);

    // update camera position
    float cx = cam_radius*sin(cam_radian);
    float cy = cam_radius*cos(cam_radian);
    vector_fill4(&cam_pos, cx, cy, 0.5, 0);
    build_camera_matrix(&cam_pos, &cam_at, &cam_up, &cameraMatrix);

    /////////////////////
    // Rendering start //
    /////////////////////

    Vector p, pProj;
    int nx, ny, lx, ly, sx, sy, sz;
    int min_x = 999, min_y = 999, max_x = 0, max_y = 0;
    lv_point_t line_points[] = {
        {0, 0},
        {0, 0},
    };

    lv_draw_line_dsc_t foreground_line_dsc;
    lv_draw_line_dsc_init(&foreground_line_dsc);
    foreground_line_dsc.color = color_render_foreground;
    foreground_line_dsc.width = 1;

    lv_draw_line_dsc_t background_line_dsc;
    lv_draw_line_dsc_init(&background_line_dsc);
    background_line_dsc.color = color_render_midground;
    background_line_dsc.width = 1;

    // starting point
    vector_fill4(&p, bangle_mem[0], bangle_mem[1], bangle_mem[2] * object_height_scale.current, 1);
    project(&p, &cameraMatrix, &projectionMatrix, &pProj);
    sx = lx = (int)((pProj.x+1)*CANVAS_RENDER_WIDTH/2);
    sy = ly = (int)((pProj.y+1)*CANVAS_RENDER_HEIGHT/2);
    sz = pProj.z;

    if (lx < min_x) min_x = lx;
    if (ly < min_y) min_y = ly;
    if (lx > max_x) max_x = lx;
    if (ly > max_y) max_y = ly;

    for (int i = 3; i < bangle_mem_point*3; i+=3) {
        vector_fill4(&p, bangle_mem[i], bangle_mem[i + 1], bangle_mem[i + 2] * object_height_scale.current, 1);
        project(&p, &cameraMatrix, &projectionMatrix, &pProj);

        nx = (int)((pProj.x+1)*CANVAS_RENDER_WIDTH/2);
        ny = (int)((pProj.y+1)*CANVAS_RENDER_HEIGHT/2);

        line_points[0].x = lx;
        line_points[0].y = ly;
        line_points[1].x = nx;
        line_points[1].y = ny;
        lv_canvas_draw_line(canvas_render, line_points, 2, pProj.z > -4 ? &foreground_line_dsc : &background_line_dsc);
        lv_canvas_set_px_color(canvas_render, lx, ly, pProj.z > -4 ? color_render_foreground : color_render_midground);

        lx = nx;
        ly = ny;

        if (lx < min_x) min_x = lx;
        if (ly < min_y) min_y = ly;
        if (lx > max_x) max_x = lx;
        if (ly > max_y) max_y = ly;
    }

    // bridge the last point and the starting point
    line_points[0].x = lx;
    line_points[0].y = ly;
    line_points[1].x = sx;
    line_points[1].y = sy;
    lv_canvas_draw_line(canvas_render, line_points, 2, sz > -4 ? &foreground_line_dsc : &background_line_dsc);
    lv_canvas_set_px_color(canvas_render, lx, ly, sz > -4 ? color_render_foreground : color_render_midground);

    // // debuging
    // uint32_t process_time = timer_read32() - last_time_tick;
    // lv_label_set_text_fmt(debug_text, "%04ld\n%04ld", delta_time, process_time);
}

void screen_render_set_key_code(uint16_t keycode, keyrecord_t *record) {
    if (!render_running) {
        return;
    }
    if (record->event.pressed) {
        animation_set_target(&object_height_scale, object_height_scale.target + object_height_scale.delta_speed*10);
    }
}

void screen_render_stop(void) {
    if (!render_running) {
        return;
    }
    render_running = false;
    lv_timer_pause(render_timer);
}

void screen_render_reload(void) {
    if (render_running) {
        return;
    }
    render_running = true;
    if (render_timer == NULL) {
        // 16 FPS, 1000/16 = 62.5
        render_timer = lv_timer_create(anim_cb, 62, NULL);
    } else {
        lv_timer_resume(render_timer);
    }
}
