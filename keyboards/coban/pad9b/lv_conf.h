// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <lv_conf.h>

// #undef LV_FONT_DEFAULT
// #define LV_FONT_DEFAULT &barlow

#undef LV_FONT_MONTSERRAT_12
#define LV_FONT_MONTSERRAT_12 0

#undef LV_FONT_MONTSERRAT_16
#define LV_FONT_MONTSERRAT_16 0

#undef LV_USE_GIF
#define LV_USE_GIF 1

#undef LV_TXT_ENC
#define LV_TXT_ENC LV_TXT_ENC_UTF8

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
