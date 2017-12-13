/**
 * @file   CInput.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip8 input subsystem (keypad)
 */
#ifndef PROJECT_CINPUT_H
#define PROJECT_CINPUT_H

#include "CMemory.h"
class CInput {
public:
  /**
   * initializes the input subsystem
   * @throw exception when keymap in configuration file has errors
   */
  CInput();

  /**
   * destructor
   */
  ~CInput();

  /**
   * get a key in the emulated keypad
   * @return the pressed key index (0-15), or -1 if no key has been pressed
   */
  int get_key_pressed();

  /**
   * check if the given key is pressed
   * @param idx index (0-15)
   * @return
   */
  bool is_key_pressed(int idx);

private:
  std::array<bool, 16> m_keypad;
  std::array<SDL_Scancode, 16> m_keymap;
};

#endif // PROJECT_CINPUT_H
