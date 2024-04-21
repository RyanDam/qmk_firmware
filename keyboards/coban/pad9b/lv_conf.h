// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <lv_conf.h>

#undef LV_FONT_DEFAULT
#define LV_FONT_DEFAULT &lv_font_montserrat_12

#undef LV_FONT_MONTSERRAT_12
#define LV_FONT_MONTSERRAT_12 1

#undef LV_FONT_MONTSERRAT_16
#define LV_FONT_MONTSERRAT_16 1

#undef LV_USE_GIF
#define LV_USE_GIF 1

// #ifdef MIN
// #undef MIN
// #endif
// #ifdef MAX
// #undef MAX
// #endif

// #undef LV_FONT_MONTSERRAT_20
// #define LV_FONT_MONTSERRAT_20 1

// #undef LV_FONT_MONTSERRAT_36
// #define LV_FONT_MONTSERRAT_36 1
