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

#include "graphics/screens/screen_animation.h"
#include "graphics/screens/styles.h"
#include "graphics/lvgl_helpers.h"

// #include "graphics/animation/nyancat_1.c"
// #include "graphics/animation/nyancat_crop.c"
// #include "graphics/animation/ghibli_crop.c"
#include "graphics/animation/ghibli5_crop.c"

static lv_obj_t *screen_animation = NULL;
static lv_obj_t *view_image;

lv_obj_t * screen_animation_init(void) {
    screen_animation = lv_obj_create(NULL);
    lv_obj_add_style(screen_animation, &style_screen, 0);
    use_flex_column(screen_animation);

    // view_image = lv_img_create(screen_animation);
    // lv_img_set_src(view_image, &nyancat_1);

    view_image = lv_gif_create(screen_animation);
    lv_gif_set_src(view_image, &ghibli5_crop);

    return screen_animation;
}
