/**
 * @file   CDisplay.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip8 display subsystem
 */

#ifndef PROJECT_CDISPLAY_H
#define PROJECT_CDISPLAY_H

#include "CMemory.h"

/** @brief screen will be drawn in white */
#define DRAW_COLOR_WHITE 0
/** @brief screen will be drawn in green */
#define DRAW_COLOR_GREEN 1
/** @brief screen will be drawn in red */
#define DRAW_COLOR_RED   2
/** @brief screen will be drawn in blue */
#define DRAW_COLOR_BLUE  3

/** @brief fixed screen width, we render in full super-chip8 mode always to easily implement half-pixel chip8 mode
 * (standard chip8 width = 64) */
#define CHIP8_WIDTH 128

/** @brief fixed screen height, we render in full super-chip8 mode always to easily implement half-pixel chip8 mode
 * (standard chip8 height = 32) */
#define CHIP8_HEIGHT 64

/**
 * implements the display subsystem
 */
class CDisplay {
public:
  /**
   * initializes the display
   * @param mem represents emulated memory
   * @throws runtime error if SDL fails
   */
  CDisplay(CMemory *mem);
  ~CDisplay();

  /**
   * updates the display
   */
  void update();

  /**
   * put a pixel at screen coordinates
   * @param x
   * @param y
   * @param on true to lit the pixel, false to unlit
   * @return
   */
  void put_pixel(int x, int y, bool on);

  /**
   * get a pixel from screen coordinates
   * @param x
   * @param y
   * @return true if the pixel is lit
   */
  bool get_pixel(int x, int y);

  /**
   * draws a sprite at screen coordinates. sprite is drawn in by XORING pixels on screen,
   * so if we set ON a pixel which was previously ON, this means a collision happened
   * @param s the sprite buffer
   * @param len buffer size
   * @param x
   * @param y
   * @return false if no collision
   */
  bool draw_sprite(const uint8_t *s, int len, int x, int y);

  /**
   * scroll down n lines
   * @param n lines to scroll down
   */
  void scroll_down(int n);

  /**
   * scroll 4 pixel left (2 pixels in chip-8 mode)
   */
  void scroll_left();

  /**
   * scroll 4 pixel right (2 pixels in chip-8 mode)
   */
  void scroll_right();

  /**
   * clear display
   */
  void clear();

  /**
   * dynamically set mode via the HIGH/LOW instruction from the cpu
   * @param mode one of the modes defined in defs.h (MODE_CHIP8, MODE_SUPERCHIP8)
   */
  void set_mode(int mode);

private:
  int m_width;
  int m_height;
  int m_mode;
  int m_color;
  double m_scale;
  SDL_Renderer *m_renderer;
  SDL_Window *m_window;
  CMemory *m_mem;
  bool m_disable_vertical_wrap;
  std::array<bool, 64 * 128> m_videomem;

  /**
   * draw sprite at screen coordinates, chip-8 mode (2x every pixel)
   * @param s the sprite buffer
   * @param lines how many lines in the sprites (8 or 16, depending on screen mode)
   * @param x
   * @param y
   * @return false if no collision
   */
  bool draw_sprite_chip8_mode(const uint8_t *s, int lines, int x, int y);
};

#endif // PROJECT_CDISPLAY_H
