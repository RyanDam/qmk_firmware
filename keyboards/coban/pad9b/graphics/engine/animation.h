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
#include <stdint.h>

typedef struct {
    float max;
    float min;
    float delta_speed; // value per sec
    float current;
    float target;

    uint32_t last_time_tick;
} AnimationValue;

void animation_init_set(AnimationValue *set, float max, float min, float delta_speed, float current, float target, uint32_t last_time_tick);

void animation_set_target(AnimationValue *set, float target);

void animation_update(AnimationValue *set, uint32_t current_time);
