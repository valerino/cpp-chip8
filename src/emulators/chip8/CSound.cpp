//
// Created by valerino on 13/11/2017.
//

#include "CSound.h"
#include <vuelib.h>

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
  Sint16 *buffer = (Sint16*)raw_buffer;
  int sample_rate=9600;
  int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
  int &sample_nr(*(int*)user_data);
  for(int i = 0; i < length; i++, sample_nr++)
  {
    double time = (double)sample_nr / (double)sample_rate;
    int hz = 1800;
    int amplitude = 20000;
    buffer[i] = (Sint16)(amplitude * sin(2.0f * M_PI * hz * time));
  }
}

CSound::CSound() {
  int sample_nr = 0;
  SDL_AudioSpec want;
  want.freq = 9600; // sample rate
  want.format = AUDIO_S16SYS; // sample type (here: signed short i.e. 16 bit)
  want.channels = 1; // only one channel
  want.samples = 2048; // buffer-size
  want.callback = audio_callback; // function SDL calls periodically to refill the buffer
  want.userdata = &sample_nr; // counter, keeping track of current sample number
  SDL_AudioSpec have;
  if(SDL_OpenAudio(&want, &have) != 0) {
    CDbg::warning("error opening audio: %s", SDL_GetError());
  }
  if(want.format != have.format) {
    CDbg::warning("audio format not supported");
  }
}

CSound::~CSound() {
  SDL_CloseAudio();
}

void CSound::beep() {
  SDL_PauseAudio(0);
  SDL_Delay(10);
  SDL_PauseAudio(1);
}
