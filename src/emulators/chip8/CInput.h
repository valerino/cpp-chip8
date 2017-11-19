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
   */
  CInput();

  ~CInput();

  /**
   * halt execution until a key is pressed
   * @return the pressed key index
   */
  int wait_key();

  /**
   * check if the given key is pressed
   * @param idx index (0-15)
   * @return true if pressed
   */
  bool is_key_pressed(int idx);

private:
  std::array<bool, 16> m_keypad;
};

#endif // PROJECT_CINPUT_H
