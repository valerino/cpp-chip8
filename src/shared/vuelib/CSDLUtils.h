//
// Created by valerino on 13/11/2017.
//

#ifndef PROJECT_CSDLUTILS_H
#define PROJECT_CSDLUTILS_H

#include <SDL2/SDL.h>

/**
 * struct t be filled by SDLPollEvents()
 */
typedef struct _sdl_keys {
  int key; /* one of the SDL_* scancodes */
  bool pressed; /* true if pressed */
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

/**
 * convert screen coordinates to flat buffer offset
 */
#define SCREEN_TO_FLAT_OFFSET(__x__, __y__, __w__) ((__y__ * __w__) + __x__)

#endif // PROJECT_CSDLUTILS_H
