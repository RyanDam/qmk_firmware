#include "quantum.h"
#include "eeprom/cb_eeprom.h"
#include "audio/voices.h"

typedef enum audio_volume_level {
    AUDIO_VOLUME_MUTE   = 0,
    AUDIO_VOLUME_LOW    = 1,
    AUDIO_VOLUME_MEDIUM = 2,
    AUDIO_VOLUME_HIGH   = 3,
    AUDIO_VOLUME_MAX    = 4,
} audio_volume_level_t;

static const uint8_t audio_volume_timbre_map[] = {
    1,  // MUTE   → 0% duty cycle
    1,  // LOW    → 25% duty cycle
    10, // MEDIUM → 50% duty cycle
    25, // HIGH   → 75% duty cycle
    50, // MAX    → 100% duty cycle
};

void audio_volume_set(uint8_t level) {
    if (level > AUDIO_VOLUME_MAX) {
        level = AUDIO_VOLUME_MAX;
    }

    uint8_t timbre = audio_volume_timbre_map[level];
    voice_set_timbre(timbre);
}

uint8_t audio_volume_get(void) {
    return config.audio_volume;
}
