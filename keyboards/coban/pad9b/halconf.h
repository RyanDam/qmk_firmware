// Copyright 2023 Cole Smith (@boardsource)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include_next <halconf.h>

#undef HAL_USE_SPI
#define HAL_USE_SPI TRUE

// #undef SPI_USE_WAIT
// #define SPI_USE_WAIT TRUE

#undef HAL_USE_PWM
#define HAL_USE_PWM TRUE
