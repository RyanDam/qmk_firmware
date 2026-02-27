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

#include "graphics/engine/render_bangle.h"
#include "graphics/engine/engine.h"
#include "graphics/resources/bangle.c"

static Matrix projectionMatrix;
static Matrix cameraMatrix;
static Vector cam_pos, cam_at, cam_up;
static float counter = 0;

void render_bangle_init(painter_device_t device) {
    // init camera matrix
    vector_fill4(&cam_pos, 0, 5, 0.5, 0);
                  vector_fill4(&cam_at, 0, 0, 0, 0);
    vector_fill4(&cam_up, 0, 0, 1, 0);
    build_camera_matrix(&cam_pos, &cam_at, &cam_up, &cameraMatrix);

    // init projection matrix
    build_projection_matrix(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 2, 100, &projectionMatrix);

    // char c[64]; //size of the number
    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r1.x*100), (int)(cameraMatrix.r1.y*100), (int)(cameraMatrix.r1.z*100), (int)(cameraMatrix.r1.s*100));
    // qp_drawtext(device, 0, 20, font, c);

    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r2.x*100), (int)(cameraMatrix.r2.y*100), (int)(cameraMatrix.r2.z*100), (int)(cameraMatrix.r2.s*100));
    // qp_drawtext(device, 0, 30, font, c);

    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r3.x*100), (int)(cameraMatrix.r3.y*100), (int)(cameraMatrix.r3.z*100), (int)(cameraMatrix.r3.s*100));
    // qp_drawtext(device, 0, 40, font, c);

    // sprintf(c, "%d %d %d %d", (int)(cameraMatrix.r4.x*100), (int)(cameraMatrix.r4.y*100), (int)(cameraMatrix.r4.z*100), (int)(cameraMatrix.r4.s*100));
    // qp_drawtext(device, 0, 50, font, c);
}

void render_bangle_task(painter_device_t device) {

    // buid camera position
    float cx = 5*sin(counter);
    float cy = 5*cos(counter);
    vector_fill4(&cam_pos, cx, cy, 0.5, 0);
    build_camera_matrix(&cam_pos, &cam_at, &cam_up, &cameraMatrix);
    counter += M_PI/200;

    // render bangle
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
            qp_line(device, lx, ly, nx, ny, 0, 0, 255);
        } else {
            qp_line(device, lx, ly, nx, ny, 0, 0, 125);
        }

        lx = nx;
        ly = ny;
        // qp_rect(device, nx-1, ny-1, nx, ny, 0, 0, 255, true);
    }

    qp_line(device, lx, ly, sx, sy, 0, 0, 255);
}
