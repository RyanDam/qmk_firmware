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
#include "graphics/resources/thintel15.qff.c"
#include "graphics/resources/bangle.c"

#include "graphics/ui.h"
#include "graphics/engine.h"

static painter_device_t      oled;
static painter_font_handle_t font;

static Matrix projectionMatrix;
static Matrix cameraMatrix;
static Vector cam_pos, cam_at, cam_up;
static char c[64]; //size of the number
static float counter = 0;

void ui_init(void) {

    oled = qp_st7735_make_spi_device(SCREEN_HEIGHT, SCREEN_WIDTH, OLED_CS_PIN, OLED_DC_PIN, OLED_RST_PIN, 4, 0);
    qp_init(oled, QP_ROTATION_270);
    qp_set_viewport_offsets(oled, 0, 24);
    qp_rect(oled, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, true);

    font = qp_load_font_mem(font_thintel15);

    // init camera matrix
    vector_fill4(&cam_pos, 0, 5, 0.5, 0);
                  vector_fill4(&cam_at, 0, 0, 0, 0);
    vector_fill4(&cam_up, 0, 0, 1, 0);
    build_camera_matrix(&cam_pos, &cam_at, &cam_up, &cameraMatrix);

    // init projection matrix
    build_projection_matrix(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 2, 100, &projectionMatrix);

    // char c[64]; //size of the number
    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r1.x*100), (int)(cameraMatrix.r1.y*100), (int)(cameraMatrix.r1.z*100), (int)(cameraMatrix.r1.s*100));
    // qp_drawtext(oled, 0, 20, font, c);

    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r2.x*100), (int)(cameraMatrix.r2.y*100), (int)(cameraMatrix.r2.z*100), (int)(cameraMatrix.r2.s*100));
    // qp_drawtext(oled, 0, 30, font, c);

    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r3.x*100), (int)(cameraMatrix.r3.y*100), (int)(cameraMatrix.r3.z*100), (int)(cameraMatrix.r3.s*100));
    // qp_drawtext(oled, 0, 40, font, c);

    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r4.x*100), (int)(cameraMatrix.r4.y*100), (int)(cameraMatrix.r4.z*100), (int)(cameraMatrix.r4.s*100));
    // qp_drawtext(oled, 0, 50, font, c);
}

void ui_task(void) {

    static uint32_t last_draw = 0;
    uint32_t        now       = timer_read32();
    if (TIMER_DIFF_32(now, last_draw) < SCREEN_REFRESH_GAP_MS) // Throttle to 30fps
        return;

    qp_rect(oled, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, true);

    float cx = 5*sin(counter);
    float cy = 5*cos(counter);
    vector_fill4(&cam_pos, cx, cy, 0.5, 0);
    build_camera_matrix(&cam_pos, &cam_at, &cam_up, &cameraMatrix);
    counter += M_PI/200;

    // char c[64]; //size of the number
    // sprintf(c, "%d   %d", (int)cx, (int)cy);
    // qp_drawtext(oled, 0, 20, font, c);

    Vector p, pProj;
    int nx, ny, lx, ly, sx, sy;
    // starting point
    vector_fill4(&p, bangle_mem[0], bangle_mem[1], bangle_mem[2], 1);
    project(&p, &cameraMatrix, &projectionMatrix, &pProj);
    lx = (int)((pProj.x+1)*SCREEN_WIDTH/2);
    ly = (int)((pProj.y+1)*SCREEN_HEIGHT/2);
    sx = lx;
    sy = ly;

    for (int i = 3; i < bangle_mem_point*3; i+=3) {
        vector_fill4(&p, bangle_mem[i], bangle_mem[i + 1], bangle_mem[i + 2], 1);
        project(&p, &cameraMatrix, &projectionMatrix, &pProj);

        nx = (int)((pProj.x+1)*SCREEN_WIDTH/2);
        ny = (int)((pProj.y+1)*SCREEN_HEIGHT/2);

        if (pProj.z > -4) {
            qp_line(oled, lx, ly, nx, ny, 0, 0, 255);
        } else {
            qp_line(oled, lx, ly, nx, ny, 0, 0, 125);
        }

        lx = nx;
        ly = ny;
        // qp_rect(oled, nx-1, ny-1, nx, ny, 0, 0, 255, true);
    }

    qp_line(oled, lx, ly, sx, sy, 0, 0, 255);

    uint32_t last       = timer_read32();
    // uint32_t total_draw = TIMER_DIFF_32(last, now);
    // sprintf(c, "%ld ms", total_draw);
    qp_drawtext(oled, 5, 5, font, c);

    last_draw = last;
}
