RAW_ENABLE = yes
AUDIO_DRIVER = pwm_hardware

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7735_spi
QUANTUM_PAINTER_LVGL_INTEGRATION = yes

# SRC += graphics/engine/engine.c
# SRC += graphics/engine/animation.c
# SRC += graphics/engine/render_bangle.c
SRC += graphics/ui.c
SRC += graphics/lvgl_helpers.c
SRC += hid/hid.c
SRC += eeprom/cb_eeprom.c
SRC += utils/keycode_lookup.c
SRC += graphics/screens/styles.c
# SRC += graphics/screens/screen_hardware_stats.c
SRC += graphics/screens/screen_time.c
SRC += graphics/screens/screen_animation.c
SRC += graphics/screens/screen_layers.c
SRC += graphics/screens/screen_boot.c
# SRC += graphics/screens/screen_render.c
SRC += graphics/screens/screen_pomodoro.c

SRC += graphics/fonts/barlow.c
SRC += graphics/fonts/inter.c
SRC += graphics/fonts/bai_jamjuree.c
SRC += graphics/fonts/dm_serif.c

OPT_DEFS = -O2
