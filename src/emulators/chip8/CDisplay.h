//
// Created by valerino on 09/11/2017.
//

#ifndef PROJECT_CDISPLAY_H
#define PROJECT_CDISPLAY_H

#include "CMemory.h"

// the default display scale factor
#define DEFAULT_SCALE_FACTOR 10.0

/**
 * implements the display subsystem
 */
class CDisplay {
public:
  /**
   * initializes the display
   * @param mode MODE_CHIP8, MODE_SUPERCHIP8, MODE_ETI660
   * @param mem represents emulated memory
   */
  CDisplay(int mode, CMemory *mem);
  ~CDisplay();

  /**
   * update the display
   */
  void update();

  /**
   * put a pixel at screen coordinates
   * @param x
   * @param y
   * @param on true to lit the pixel, false to unlit
   * @return 1 if the pixel was set and is overwritten (=collision)
   */
  int put_pixel(int x, int y, bool on);

  /**
   * get a pixel from screen coordinates
   * @param x
   * @param y
   * @return true if the pixel is lit
   */
  bool get_pixel(int x, int y);

  /**
   * draw sprite at screen coordinates
   * @param s the sprite buffer
   * @param len buffer size
   * @param x
   * @param y
   * @return 0 if no collision
   */
  int draw_sprite(const uint8_t *s, int len, int x, int y);

  /**
   * scroll down N lines
   * @param n lines to scroll down
   */
  void scroll_down(int n);

  /**
   * scroll 4 pixel left
   */
  void scroll_left();

  /**
   * scroll 4 pixel right
   */
  void scroll_right();

  /**
   * clear display
   */
  void clear();

  /**
   * dynamically set mode via the HIGH/LOW instruction from the cpu
   * @param mode MODE_CHIP MODE_SUPERCHIP8
   */
  void set_mode(int mode);

private:
  CMemory *m_mem;
  int m_mode;
  int m_width;
  int m_height;
  SDL_Renderer *m_renderer;
  SDL_Window *m_window;
  std::array<bool, 64 * 128> m_videomem;
};

#endif // PROJECT_CDISPLAY_H
