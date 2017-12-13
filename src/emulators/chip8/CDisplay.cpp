/**
 * @file   CDisplay.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip8 display subsystem
 */

#include <vuelib.h>
#include "CDisplay.h"
#include "defs.h"

CDisplay::CDisplay(CMemory *mem)
    : m_scale(10.0), m_color(DRAW_COLOR_GREEN), m_mode(MODE_CHIP8), m_mem{mem}, m_height{CHIP8_HEIGHT},
      m_width{CHIP8_WIDTH}, m_videomem{}, m_disable_vertical_wrap{false} {

  // do we want fullscreen ?
  // TODO: fix appearance
  uint32_t flags = 0;
  if (CConfiguration::instance()->get<bool>("display_fullscreen")) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  // get the wanted pixel draw color (white or green)
  std::string color = CConfiguration::instance()->get<std::string>("display_draw_color");
  if (color == "white") {
    m_color = DRAW_COLOR_WHITE;
  } else if (color == "red") {
    m_color = DRAW_COLOR_RED;
  } else if (color == "blue") {
    m_color = DRAW_COLOR_BLUE;
  } else {
    // default, green
    m_color = DRAW_COLOR_GREEN;
  }

  // check for display fixes to be applied
  m_disable_vertical_wrap = CConfiguration::instance()->get<bool>("fix_disable_vertical_wrap");

  // get the scale to apply
  m_scale = CConfiguration::instance()->get<double>("display_scale");

  // create the window
  std::string title = std::string("vue[chip8]: " + m_mem->path());
  m_window =
      SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       m_width * m_scale, m_height * m_scale, flags | SDL_WINDOW_OPENGL);
  if (!m_window) {
    // error creating window
    const char *msg = SDL_GetError();
    throw std::runtime_error(msg);
  }

  // get the default renderer
  m_renderer = SDL_CreateRenderer(m_window, -1, 0);

  // scale output
  SDL_RenderSetScale(m_renderer, m_scale, m_scale);

  // clear and show window
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
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

void CDisplay::update() {
  SDL_RenderClear(m_renderer);

  // walk the video memory
  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      // is the pixel set ?
      bool is_on = get_pixel(x, y);
      if (is_on) {
        // pixel is on
        if (m_color == DRAW_COLOR_WHITE) {
          SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        } else if (m_color == DRAW_COLOR_BLUE) {
          SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        } else if (m_color == DRAW_COLOR_RED) {
          SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        } else {
          // default is green
          SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        }
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

void CDisplay::put_pixel(int x, int y, bool on) {
  int pos = SCREEN_TO_FLAT_OFFSET(x, y, m_width);
  m_videomem[pos] = m_videomem[pos] ^ on;
}

bool CDisplay::get_pixel(int x, int y) {
  int pos = SCREEN_TO_FLAT_OFFSET(x, y, m_width);
  return m_videomem[pos];
}

void CDisplay::scroll_down(int n) {
  if (n <= 0) {
    return;
  }

  /*
   * aaaaaaaaaaaaa
   * bbbbbbbbbbbbb  aaaaaaaaaaaaa
   * ccccccccccccc  bbbbbbbbbbbbb
   */

  // we have to scroll down n lines, clearing n lines on top
  int pixels = n * m_width;
  std::vector<bool> cleared(pixels, false);

  // copy n to n+1 lines
  std::copy(m_videomem.begin(), m_videomem.end() - pixels, m_videomem.begin() + pixels);

  // clear lines on top
  std::copy(cleared.begin(), cleared.end(), m_videomem.begin());
}

void CDisplay::scroll_left() {
  /*
   * aaaabbbbbbbb  bbbbbbbb
   * aaaabbbbbbbb  bbbbbbbb
   * aaaabbbbbbbb  bbbbbbbb
   */

  int pixels = 4;
  if (m_mode == MODE_CHIP8) {
    // this can be used also in chip 8 mode, just scrolls 2 pixels instead of 4
    pixels = 2;
  }

  // scroll all screen n pixels left (no overlap on the other side)
  for (int y = 0; y < m_height; y++) {
    for (int x = 0; x < m_width; x++) {
      int xx = SCREEN_TO_FLAT_OFFSET(x, y, m_width);
      if (x < (m_width - pixels)) {
        m_videomem[xx] = m_videomem[xx + pixels];
      } else {
        m_videomem[xx] = false;
      }
    }
  }
}

void CDisplay::scroll_right() {
  /*
   * aaaabbbbbbbb      aaaabbbb
   * aaaabbbbbbbb      aaaabbbb
   * aaaabbbbbbbb      aaaabbbb
   */

  int pixels = 4;
  if (m_mode == MODE_CHIP8) {
    // this can be used also in chip 8 mode, just scrolls 2 pixels instead of 4
    pixels = 2;
  }

  // scroll all screen n pixels right (no overlap on the other side)
  for (int y = 0; y < m_height; y++) {
    for (int x = m_width; x > 0; x--) {
      int xx = SCREEN_TO_FLAT_OFFSET(x, y, m_width);
      if (x > pixels) {
        m_videomem[xx] = m_videomem[xx - pixels];
      } else {
        m_videomem[xx] = false;
      }
    }
  }
}

void CDisplay::clear() { m_videomem.fill(false); }

void CDisplay::set_mode(int mode) {
  if (mode == MODE_SUPERCHIP8) {
    m_mode = MODE_SUPERCHIP8;
    // update window title to signal super-chip8 mode
    const char* t = SDL_GetWindowTitle(m_window);
    std::string title = std::string(t) + " [S-CHIP8]";
    SDL_SetWindowTitle(m_window, title.data());
  } else {
    // standard chip8
    m_mode = MODE_CHIP8;
  }

  // set new size
  SDL_SetWindowSize(m_window, m_width * m_scale, m_height * m_scale);

  // and center window
  SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool CDisplay::draw_sprite_chip8_mode(const uint8_t *s, int lines, int x, int y) {
  int collision = false;
  for (int i = 0; i < lines; i++) {

    // get sprite line, 1 byte
    uint8_t line = s[i];

    // draws 8 bit line pixel per pixel, doubling each pixel
    for (int j = 0; j < 8; j++) {
      int xx = (x + j) % (m_width / 2);
      int yy;
      if (m_disable_vertical_wrap) {
        // some games breaks (mines)when vertical wrapping is enabled....
        yy = (y + i);
      } else {
        yy = (y + i) % (m_height / 2);
      }
      if (line & (0x80 >> j)) {
        // bit is set, lit pixel at coordinates
        if (get_pixel(xx * 2, yy * 2) == true ||
            get_pixel(xx * 2, yy * 2 + 1) == true ||
            get_pixel(xx * 2 + 1, yy * 2) == true ||
            get_pixel(xx * 2 + 1, yy * 2 + 1) == true) {
          // set collision if the pixel was previously set (just check one pixel here, since its always
          // guaranteed its doubled)
          collision = true;
        }
        put_pixel(xx * 2, yy * 2, true);
        put_pixel(xx * 2, yy * 2 + 1, true);
        put_pixel(xx * 2 + 1, yy * 2, true);
        put_pixel(xx * 2 + 1, yy * 2 + 1, true);
      } else {
        // clear the pixel
        put_pixel(xx * 2, yy * 2, false);
        put_pixel(xx * 2, yy * 2 + 1, false);
        put_pixel(xx * 2 + 1, yy * 2, false);
        put_pixel(xx * 2 + 1, yy * 2 + 1, false);
      }
    }
  }
  return collision;
}

bool CDisplay::draw_sprite(const uint8_t *s, int len, int x, int y) {
  const uint8_t *ptr = s;
  bool collision = false;
  if (len == 0) {
    // drawing a sprite when len = 0, in chip8 mode we draw an 8x16 sprite, while in super-chip8 mode we draw
    // a 16x16 sprite
    if (m_mode == MODE_CHIP8) {
      // draws an 8*16 (w*h) 16 bytes sprite in chip8 mode, every pixel is doubled since
      // we render in super-chip8 mode always to easily obtain half-pixel
      return draw_sprite_chip8_mode(s, 16, x, y);
    } else {
      // super chip8 mode, draw 16x16 sprite, 32 bytes, each line is 2 bytes (16 bit)
      for (int i = 0; i < 16; i++) {
        // get sprite line, 2 bytes
        uint16_t line = (ptr[0] << 8) | ptr[1];

        // draw 16 bit line pixel per pixel
        for (int j = 0; j < 16; j++) {
          int xx = (x + j) % m_width;
          int yy;
          if (m_disable_vertical_wrap) {
            // some games breaks (mines)when vertical wrapping is enabled....
            yy = (y + i);
          } else {
            yy = (y + i) % m_height;
          }
          if (line & (0x8000 >> j)) {
            // bit is set, lit pixel at coordinates
            if (get_pixel(xx, yy) == true) {
              // set collision if the pixel was previously set
              collision = true;
            }
            put_pixel(xx, yy, true);
          } else {
            // clear the pixel
            put_pixel(xx, yy, false);
          }
        }
        // next line
        ptr += 2;
      }
    }
  } else {
    // drawing a sprite when len != 0, in chip8 mode we draw a 8*len sprite in every mode
    if (m_mode == MODE_CHIP8) {
      // draws an 8*len (w*h) sprite of size=len bytes in chip8 mode, every pixel is doubled since
      // we render in super-chip8 mode always to easily obtain half-pixel
      return draw_sprite_chip8_mode(s, len, x, y);
    } else {
      // draws an 8*len (w*h) sprite in super-chip8 mode
      for (int i = 0; i < len; i++) {
        // get sprite line, 1 byte
        uint8_t line = ptr[i];

        // draw 8 bit line pixel per pixel
        for (int j = 0; j < 8; j++) {
          int xx = (x + j) % m_width;
          int yy;
          if (m_disable_vertical_wrap) {
            // some games breaks (mines)when vertical wrapping is enabled....
            yy = (y + i);
          } else {
            yy = (y + i) % m_height;
          }
          if (line & (0x80 >> j)) {
            // bit is set, lit pixel at coordinates
            if (get_pixel(xx, yy) == true) {
              // set collision if the pixel was previously set
              collision = true;
            }
            put_pixel(xx, yy, true);
          } else {
            // clear the pixel
            put_pixel(xx, yy, false);
          }
        }
      }
    }
  }
  return collision;
}
