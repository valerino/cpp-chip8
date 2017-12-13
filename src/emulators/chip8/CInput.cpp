//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CInput.h"

CInput::CInput() : m_keypad{} {
  // read key mapping from configuration
  // each key is a string value, i.e. "a", "b", "0", "1" etc... representing the 0..F chip8 keypad
  m_keymap[0]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_0").data());
  m_keymap[1]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_1").data());
  m_keymap[2]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_2").data());
  m_keymap[3]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_3").data());
  m_keymap[4]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_4").data());
  m_keymap[5]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_5").data());
  m_keymap[6]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_6").data());
  m_keymap[7]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_7").data());
  m_keymap[8]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_8").data());
  m_keymap[9]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_9").data());
  m_keymap[10]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_a").data());
  m_keymap[11]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_b").data());
  m_keymap[12]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_c").data());
  m_keymap[13]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_d").data());
  m_keymap[14]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_e").data());
  m_keymap[15]=SDL_GetScancodeFromName(CConfiguration::instance()->get<std::string>("key_f").data());
  for (int i=0; i < m_keymap.size(); i++) {
    if (m_keymap[i] == SDL_SCANCODE_UNKNOWN || m_keymap[i] == SDL_SCANCODE_P || m_keymap[i] == SDL_SCANCODE_ESCAPE) {
      throw std::system_error(ENOENT, std::generic_category(),
                              std::string("configuration 'key_x' error (remember, 'ESC' and 'P' are reserved and can't be mapped"));
    }
  }
}

CInput::~CInput() {}

bool CInput::is_key_pressed(int idx) {
  // read keyboard state
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  return state[m_keymap[idx]];
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
