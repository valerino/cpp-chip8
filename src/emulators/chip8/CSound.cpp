/**
 * @file   CSound.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip-8 sound subsystem
 */

#include <vuelib.h>
#include "CSound.h"

/**
 * waveform data
 */
struct audiodata_t {
  /** @brief sample position, must be initialized to 0 */
  float tone_pos;
  /** @brief must be initialized with the waveform start seed */
  float tone_inc;
};

/**
 * audio callback which generates the beep (raw PCM 44100hz)
 * @param udata initialized with waveform start seed
 * @param stream the sample output buffer
 * @param len size of the sample output buffer
 */
void feed(void *udata, Uint8 *stream, int len) {
  struct audiodata_t *audio = (struct audiodata_t *)udata;
  for (int i = 0; i < len; i++) {
    stream[i] = sinf(audio->tone_pos) + 127;
    audio->tone_pos += audio->tone_inc;
  }
}

CSound::CSound() :
  m_device{0},m_spec{NULL} {

  // initializes waveform data for raw audio PCM generation
  struct audiodata_t *audio =
      (struct audiodata_t *)CMem::alloc(sizeof(struct audiodata_t));
  audio->tone_pos = 0;
  audio->tone_inc = 2 * 3.14159 * 1000 / 44100;

  // set up the audiospec data structure required by SDL
  m_spec = (SDL_AudioSpec *)CMem::alloc(sizeof(SDL_AudioSpec));
  m_spec->freq = 44100;
  m_spec->format = AUDIO_U8;
  m_spec->channels = 1;
  m_spec->samples = 4096;
  m_spec->callback = feed;
  m_spec->userdata = audio;

  // open default device
  m_device =
      SDL_OpenAudioDevice(NULL, 0, m_spec, m_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
}

CSound::~CSound() {
  CMem::free(m_spec->userdata);
  CMem::free(m_spec);
  if (m_device) {
    // close device
    SDL_CloseAudioDevice(m_device);
  }
}

void CSound::beep(bool enable) {
  if (enable) {
    // let the beep run
    SDL_PauseAudioDevice(m_device,0);
  } else {
    // mute the beep
    SDL_PauseAudioDevice(m_device,1);
  }
}
