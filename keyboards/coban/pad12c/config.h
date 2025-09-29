// Copyright 2025 RyanDam (https://github.com/RyanDam)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

// EC 11, 30 detent -> 15 pulse
// Kailh Encoder, 24 detent -> 12 pulse
#define ENCODER_RESOLUTIONS { 4, 2 }

