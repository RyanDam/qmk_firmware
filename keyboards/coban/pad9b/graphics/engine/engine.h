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

#include <math.h>

typedef struct {
    float x, y, z;
    float s;
} Vector;

typedef struct {
    Vector r1, r2, r3, r4;
} Matrix;

// Belows is vector construction functions
void vector_fill4(Vector *a, float x, float y, float z, float s);

void vector_fill1(Vector *a, float v);

void vector_fillv(Vector *a, Vector *b);

// Belows is vector maths
void vector_cross(Vector *a, Vector *b, Vector *r);

void vector_devide(Vector *a, float s, Vector *r);

void vector_norm(Vector *a, Vector *r);

void vector_minus(Vector *a, Vector *b, Vector *r);

float vector_dot3(Vector *a, Vector *b);

float vector_dot4(Vector *a, Vector *b);

// Belows is very simple graphic functions
void matrix_mul(Matrix *m, Vector *v, Vector *r);

void build_camera_matrix(Vector *pos, Vector *at, Vector *up, Matrix *r);

void build_projection_matrix(int screen_width, int screen_height, float zNear, float zFar, float fov, Matrix *r);

void project(Vector *p, Matrix *camMatrix, Matrix *ProMatrix, Vector *r);
