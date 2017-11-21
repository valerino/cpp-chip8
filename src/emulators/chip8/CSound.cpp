//
// Created by valerino on 13/11/2017.
//

#include "CSound.h"
#include <vuelib.h>

/**
 * This is a private structure used for holding information about audio.
 * I need to create the structure becuase the feeding function for audio
 * in SDL only allows one single parameter to be provided via user data.
 * This little trick lets me pass more than one variable.
 */
struct audiodata_t {
  float tone_pos;
  float tone_inc;
};

/**
 * This is the function that generates the beep noise heard in the emulator.
 * It generates RAW PCM values that are written to the stream. This is fast
 * and has no dependencies on external files.
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

  /* Initialize user data structure. */
  struct audiodata_t *audio =
      (struct audiodata_t *)CMem::alloc(sizeof(struct audiodata_t));
  audio->tone_pos = 0;
  audio->tone_inc = 2 * 3.14159 * 1000 / 44100;

  /* Set up the audiospec data structure required by SDL. */
  m_spec = (SDL_AudioSpec *)CMem::alloc(sizeof(SDL_AudioSpec));
  m_spec->freq = 44100;
  m_spec->format = AUDIO_U8;
  m_spec->channels = 1;
  m_spec->samples = 4096;
  m_spec->callback = feed;
  m_spec->userdata = audio;
  m_device =
      SDL_OpenAudioDevice(NULL, 0, m_spec, m_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  int r = 9;
}

CSound::~CSound() {
  CMem::free(m_spec->userdata);
  CMem::free(m_spec);
  if (m_device) {
    SDL_CloseAudioDevice(m_device);
  }
}

void CSound::beep(bool enable) {
  if (enable) {
    SDL_PauseAudioDevice(m_device,0);
  } else {
    SDL_PauseAudioDevice(m_device,1);
  }
}
