//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CDisplay.h"
#include "defs.h"

CDisplay::CDisplay(int mode) {
  int h=640, w = 480;
  if (mode == MODE_SUPER_CHIP8) {

  }

  // create window
  m_window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
  if (!m_window) {
    CDbg::error(SDL_GetError());
    return;
  }

  // get the default renderer
  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  m_surface = SDL_GetWindowSurface(m_window);

  // set to black and show
  SDL_RenderClear(m_renderer);
  SDL_RenderPresent(m_renderer);
}

CDisplay::~CDisplay() {
  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
  }
  if (m_window) {
    SDL_DestroyWindow(m_window);
  }

}
