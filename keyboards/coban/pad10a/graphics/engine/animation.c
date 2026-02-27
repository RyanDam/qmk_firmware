#include <math.h>
#include "graphics/engine/animation.h"

#define FLOAT_ABSOLUTE_DELTA 0.0001f

void animation_init_set(AnimationValue *set, float max, float min, float delta_speed, float current, float target, uint32_t last_time_tick) {
    set->max = max;
    set->min = min;
    set->delta_speed = delta_speed;
    set->current = current;
    set->target = target;
    set->last_time_tick = last_time_tick;
}

void animation_set_target(AnimationValue *set, float target) {
    if (target > set->max) {
        set->target = set->max;
    } else if (target < set->min) {
        set->target = set->min;
    } else {
        set->target = target;
    }
}

void animation_update(AnimationValue *set, uint32_t current_time) {
    uint32_t delta_time = current_time - set->last_time_tick;
    if (delta_time == 0) return;
    set->last_time_tick = current_time;

    if (fabsf(set->current - set->target) < FLOAT_ABSOLUTE_DELTA) return;

    float new_change = set->delta_speed * delta_time/1000;
    new_change *= set->target > set->current ? 1 : -1; // Inclusive zero

    if (set->current + new_change > set->max) {
        set->current = set->max;
    } else if (set->current + new_change < set->min) {
        set->current = set->min;
    } else {
        set->current += new_change;
    }
}
