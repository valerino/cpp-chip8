//
// Created by valerino on 09/11/2017.
//

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

  ~CInput();

  /**
   * get a key
   * @return the pressed key index, or -1 if no key has been pressed
   */
  int is_key_pressed();

  /**
   * check if the given key is pressed
   * @param idx index (0-15)
   * @return true if pressed
   */
  bool is_key_pressed(int idx);

private:
  std::array<bool, 16> m_keypad;
  std::array<SDL_Scancode, 16> m_keymap;
};

#endif // PROJECT_CINPUT_H
