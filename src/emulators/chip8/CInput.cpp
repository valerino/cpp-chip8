//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CInput.h"

CInput::CInput() : m_keypad{} {}

CInput::~CInput() {}

bool CInput::is_key_pressed(int idx) {
  // read keyboard state
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  bool pressed = false;
  switch (idx) {
  case 0:
    pressed = state[SDL_SCANCODE_0];
    break;
  case 1:
    pressed = state[SDL_SCANCODE_1];
    break;
  case 2:
    pressed = state[SDL_SCANCODE_2];
    break;
  case 3:
    pressed = state[SDL_SCANCODE_3];
    break;
  case 4:
    pressed = state[SDL_SCANCODE_4];
    break;
  case 5:
    pressed = state[SDL_SCANCODE_5];
    break;
  case 6:
    pressed = state[SDL_SCANCODE_6];
    break;
  case 7:
    pressed = state[SDL_SCANCODE_7];
    break;
  case 8:
    pressed = state[SDL_SCANCODE_8];
    break;
  case 9:
    pressed = state[SDL_SCANCODE_9];
    break;
  case 10:
    pressed = state[SDL_SCANCODE_A];
    break;
  case 11:
    pressed = state[SDL_SCANCODE_B];
    break;
  case 12:
    pressed = state[SDL_SCANCODE_C];
    break;
  case 13:
    pressed = state[SDL_SCANCODE_D];
    break;
  case 14:
    pressed = state[SDL_SCANCODE_E];
    break;
  case 15:
    pressed = state[SDL_SCANCODE_F];
    break;
  default:
    break;
  }
  return pressed;
}

int CInput::is_key_pressed() {
  for (int i = 0; i < 16; i++) {
    bool pressed = is_key_pressed(i);
    if (pressed) {
      return i;
    }
  }
  return -1;
}
