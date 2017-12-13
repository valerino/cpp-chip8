/**
 * @file   CInputUtils.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  input related utilities, part of vuelib
 */
#include "CInputUtils.h"
#include <errno.h>
#include <SDL2/SDL.h>

int CSDLUtils::sdl_poll_events(uint8_t **keys) {
  if (!keys) {
    return EINVAL;
  }
  *keys = NULL;

  // run an SDL poll cycle
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_QUIT) {
      // SDL window closed, application must quit asap
      return 1;
    } else if (ev.type == SDL_KEYDOWN) {
      // a key has been pressed, return the keyboard state
      uint8_t *state = const_cast<uint8_t *>(SDL_GetKeyboardState(NULL));
      *keys = state;
    }
  }
  return 0;
}
