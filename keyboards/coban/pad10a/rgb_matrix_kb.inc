RGB_MATRIX_EFFECT(screen_pomo_noti_effect)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static uint32_t screen_pomo_noti_effect_state;
static void screen_pomo_noti_effect_init(effect_params_t* params) {
  screen_pomo_noti_effect_state = timer_read32();
}
static bool screen_pomo_noti_effect_run(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  uint32_t current_time = timer_read32();
  uint32_t eslapsed_time = current_time - screen_pomo_noti_effect_state;

  float state = sin(10*eslapsed_time*0.0012)*sin(6*eslapsed_time*0.0012);
  uint8_t val = state >= 0.5 ? rgb_matrix_config.hsv.v : 0x00;

  hsv_t hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, val};
  rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
  rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
  return rgb_matrix_check_finished_leds(led_max);
}
static bool screen_pomo_noti_effect(effect_params_t* params) {
  if (params->init) screen_pomo_noti_effect_init(params);
  return screen_pomo_noti_effect_run(params);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
