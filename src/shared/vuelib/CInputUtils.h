/**
 * @file   CSDLUtils.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  input related utilities, part of vuelib
 */

#ifndef PROJECT_CSDLUTILS_H
#define PROJECT_CSDLUTILS_H

#include <stdint.h>

/**
 * struct t be filled by sdl_poll_events()
 */
typedef struct _sdl_keys {
  /** @brief one of the SDL_* scancodes */
  int key;
  /** @brief key state */
  bool pressed;
} sdl_keys;

class CSDLUtils {
public:
  /**
   * poll sdl events and return keyboard state
   * @param keys on return, if not NULL is the keyboard state returned by SDL_GetKeyboardState()
   * @return != 0 if the app must quit asap
   */
  static int sdl_poll_events(uint8_t **keys);
};

#endif // PROJECT_CSDLUTILS_H
