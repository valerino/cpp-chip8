//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CDisplay.h"
#include "CGfx.h"

#include "defs.h"

/**
 * convert screen coordinates to flat buffer offset
 */
#define SCREEN_TO_FLAT_OFFSET(__x__, __y__, __w__) ((__y__ * __w__) + __x__)

CDisplay::CDisplay(int mode, CMemory *mem)
    : m_mode{mode}, m_mem{mem}, m_height{32}, m_width{64}, m_videomem{} {
  if (m_mode == MODE_SUPER_CHIP8) {
    m_height = 64;
    m_width = 128;
  }

  // create window
  m_window =
      SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       m_width * DEFAULT_SCALE_FACTOR,
                       m_height * DEFAULT_SCALE_FACTOR, SDL_WINDOW_SHOWN);
  if (!m_window) {
    CDbg::error(SDL_GetError());
    return;
  }

  // get the default renderer
  m_renderer = SDL_CreateRenderer(m_window, -1, 0);

  // adjust the scaling factor
  SDL_RenderSetScale(m_renderer, DEFAULT_SCALE_FACTOR, DEFAULT_SCALE_FACTOR);

  // clear and show window
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
  SDL_RenderClear(m_renderer);
  SDL_RenderPresent(m_renderer);
  SDL_RaiseWindow(m_window);
}

CDisplay::~CDisplay() {
  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
  }
  if (m_window) {
    SDL_DestroyWindow(m_window);
  }
}

void CDisplay::update() {
  SDL_RenderClear(m_renderer);

  // walk the video memory
  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      // is the pixel set ?
      bool is_on = get_pixel(x, y);
      if (is_on == true) {
        // pixel is on
        SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
      } else {
        // pixel is off
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      }

      // draw pixel
      SDL_RenderDrawPoint(m_renderer, x, y);
    }
  }
  SDL_RenderPresent(m_renderer);
}

int CDisplay::put_pixel(int x, int y, bool on) {
  int pos = SCREEN_TO_FLAT_OFFSET(x, y, m_width);
  // xor pixel on screen
  // 1 xor 0 = 1
  // 1 xor 1 = 0
  // 0 xor 0 = 0
  bool b = m_videomem[pos] ^ on;
  m_videomem[pos] = b;
  return !((int)b);
}

bool CDisplay::get_pixel(int x, int y) {
  int pos = SCREEN_TO_FLAT_OFFSET(x, y, m_width);
  return m_videomem[pos];
}

void CDisplay::scroll_down(int n) {
  // save n lines
  int to_save = n * m_width;
  std::vector<bool> saved(to_save);
  memcpy(&saved, &m_videomem, to_save);

  // shift down whole buffer
  memcpy(&m_videomem, &m_videomem + to_save, m_videomem.size() - to_save);

  // overlap
  memcpy(&m_videomem + m_videomem.size() - to_save, &saved, to_save);
}

void CDisplay::scroll_left() {
  // save 4 bytes
  std::array<bool, 4> saved;
  memcpy(&saved, &m_videomem, 4);

  // shift whole screen left
  memcpy(&m_videomem, &m_videomem + 4, m_videomem.size() - 4);

  // overlap
  memcpy(&m_videomem + m_videomem.size() - 4, &saved, 4);
}

void CDisplay::scroll_right() {
  // save 4 bytes
  std::array<bool, 4> saved;
  memcpy(&saved, &m_videomem + m_videomem.size() - 4, 4);

  // shift whole screen right
  memcpy(&m_videomem + 4, &m_videomem, m_videomem.size() - 4);

  // overlap
  memcpy(&m_videomem, &saved, 4);
}
void CDisplay::clear() { m_videomem.fill(false); }

void CDisplay::set_mode(int mode) {
  m_mode = mode;
  if (mode == MODE_SUPER_CHIP8) {
    m_height = 64;
    m_width = 128;
  } else {
    m_height = 32;
    m_width = 64;
  }
  SDL_SetWindowSize(m_window, m_width * DEFAULT_SCALE_FACTOR,
                    m_height * DEFAULT_SCALE_FACTOR);
}

int CDisplay::draw_sprite(const uint8_t *s, int len, int x, int y) {
  // Dxyn - DRW Vx, Vy, nibble

  const uint8_t *ptr = s;
  int collision = 0;
  if (len == 0 && m_mode == MODE_SUPER_CHIP8) {
    // draw 16x16 sprite, each line is 2 bytes (16 bit)
    for (int i = 0; i < 16; i++) {
      // get sprite line
      uint16_t line;
      memcpy(&line, ptr, 2);

      // draw 16 bit line pixel per pixel
      for (int j = 0; j < 16; j++) {
        // handle overlapping
        int xx = x + j;
        if (xx > m_width) {
          xx = xx - m_width;
        }

        int yy = y + i;
        if (yy > m_height) {
          yy = yy - m_height;
        }

        if (line & (0x8000 >> j)) {
          // bit is set, lit pixel at coordinates
          if (put_pixel(xx, yy, true)) {
            // if it was on before (pixel is xored on screen), there was a
            // collision
            collision++;
          }
        }// else {
          // clear the pixel
          //put_pixel(xx, yy, false);
        //}
      }
      // next line
      ptr += 2;
    }
  } else {
    // draw 8*len sprite
    for (int i = 0; i < len; i++) {
      // get sprite byte
      uint8_t b = ptr[i];

      // draw 8 bit line pixel per pixel
      for (int j = 0; j < 8; j++) {
        // handle overlapping
        int xx = x + j;
        if (xx > m_width) {
          // w=60 xx=61
          xx = xx - m_width;
        }

        int yy = y + i;
        if (yy > m_height) {
          yy = yy - m_height;
        }

        if (b & (0x80 >> j)) {
          // bit is set, lit pixel at coordinates
          if (put_pixel(xx, yy, true)) {
            // if it was on before (pixel is xored on screen), there was a
            // collision
            collision++;
          }
        } else {
          // clear the pixel
          put_pixel(xx, yy, false);
        }
      }
    }
  }
  return collision;
}
