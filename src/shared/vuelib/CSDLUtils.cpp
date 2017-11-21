//
// Created by valerino on 13/11/2017.
//

#include "CSDLUtils.h"

int CSDLUtils::SDLPollEvents(uint8_t** keys) {
  *keys = NULL;

  // sdl_poll sdl events
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_QUIT) {
      return 1;
    } else if (ev.type == SDL_KEYDOWN) {
      // get keyboard state
      uint8_t *state = const_cast<uint8_t *>(SDL_GetKeyboardState(NULL));
      *keys = state;
    }
  }
  return 0;
}
