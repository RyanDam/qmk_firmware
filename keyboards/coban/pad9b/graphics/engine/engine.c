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

#include "graphics/engine/engine.h"

void vector_fill4(Vector *a, float x, float y, float z, float s) {
    a->x = x;
    a->y = y;
    a->z = z;
    a->s = s;
}

void vector_fill1(Vector *a, float v) {
    vector_fill4(a, v, v, v, v);
}

void vector_fillv(Vector *a, Vector *b) {
    vector_fill4(a, b->x, b->y, b->z, b->s);
}

void vector_cross(Vector *a, Vector *b, Vector *r) {
    r->x = a->y*b->z - a->z*b->y;
    r->y = a->z*b->x - a->x*b->z;
    r->z = a->x*b->y - a->y*b->x;
}

void vector_devide(Vector *a, float s, Vector *r) {
    r->x = a->x/s;
    r->y = a->y/s;
    r->z = a->z/s;
}

void vector_norm(Vector *a, Vector *r) {
    float l = sqrt(a->x*a->x + a->y*a->y + a->z*a->z);
    vector_devide(a, l, r);
}

void vector_minus(Vector *a, Vector *b, Vector *r) {
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
}

float vector_dot3(Vector *a, Vector *b) {
    return a->x*b->x + a->y*b->y + a->z*b->z;
}

float vector_dot4(Vector *a, Vector *b) {
    return a->x*b->x + a->y*b->y + a->z*b->z + a->s*b->s;
}

void matrix_mul(Matrix *m, Vector *v, Vector *r) {
    r->x = vector_dot4(&(m->r1), v);
    r->y = vector_dot4(&(m->r2), v);
    r->z = vector_dot4(&(m->r3), v);
    r->s = vector_dot4(&(m->r4), v);
}

void build_camera_matrix(Vector *pos, Vector *at, Vector *up, Matrix *r) {
    Vector target;
    vector_minus(at, pos, &target);
    target.s = 0;

    Vector N;
    vector_norm(&target, &N);
    N.s = 0;

    Vector Un, Uc, U;
    vector_norm(up, &Un);
    vector_cross(&Un, &target, &Uc);
    vector_norm(&Uc, &U);
    U.s = 0;

    Vector Vc, V;
    vector_cross(&N, &U, &Vc);
    vector_norm(&Vc, &V);
    V.s = 0;

    Vector S;
    vector_fill4(&S, 0, 0, 0, 1);

    vector_fillv(&(r->r1), &U);
    vector_fillv(&(r->r2), &V);
    vector_fillv(&(r->r3), &N);
    vector_fillv(&(r->r4), &S);
}

void build_projection_matrix(int screen_width, int screen_height, float zNear, float zFar, float fov, Matrix *r) {
    float ratio = (float)screen_width/screen_height;
    float zRange = zNear - zFar;
    float tanHFov = tanf((M_PI/180.0)*fov/2.0);

    r->r1.x = 1/(tanHFov*ratio);
    r->r1.y = 0;
    r->r1.z = 0;
    r->r1.s = 0;

    r->r2.x = 0;
    r->r2.y = 1/tanHFov;
    r->r2.z = 0;
    r->r2.s = 0;

    r->r3.x = 0;
    r->r3.y = 0;
    r->r3.z = (-zNear-zFar)/zRange;
    r->r3.s = 2*zFar*zNear/zRange;

    r->r4.x = 0;
    r->r4.y = 0;
    r->r4.z = 1;
    r->r4.s = 0;
}

void project(Vector *p, Matrix *camMatrix, Matrix *ProMatrix, Vector *r) {
    Vector pCam;
    matrix_mul(camMatrix, p, &pCam);
    matrix_mul(ProMatrix, &pCam, r);
}
