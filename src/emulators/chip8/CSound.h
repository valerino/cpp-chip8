//
// Created by valerino on 13/11/2017.
//

#ifndef PROJECT_CSOUND_H
#define PROJECT_CSOUND_H

#include <SDL2/SDL_audio.h>

/**
 * implements the sound subsystem
 * the chip8 has no real sound subsystem, it just beeps when the sound timer reaches 0
 */
class CSound {
public:
  /**
   * initializes SDL audio
   */
  CSound();

  /**
   * destructor
   */
  virtual ~CSound();

  /**
   * emit beep
   * @param enable true to start beeping, false to stop
   */
  void beep(bool enable);

private:
  SDL_AudioSpec* m_spec;
  SDL_AudioDeviceID m_device;
};

#endif // PROJECT_CSOUND_H
