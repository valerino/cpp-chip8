//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "defs.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CMemory.h"
#include "CCpu.h"
#include "CEmuChip8.h"
#include "CDisplay.h"

std::string CEmuChip8::name() {
  return std::string("chip8");
}

int CEmuChip8::start(const char *rom_path, const char *extras_path) {
  // TODO: read files
  // TODO: read configuration
  int mode = MODE_CHIP8;

  // initialize SDL
  int res = SDL_Init(SDL_INIT_EVERYTHING);
  if (res != 0) {
    CDbg::error("%s", SDL_GetError());
    return 1;
  }
  CDbg::notify("SDL initialized");

  // initialize display
  m_display = new CDisplay(mode);

  // sleep a bit
  SDL_Delay(5000);
  //while (1) {
  //  sleep(1);
  //}

  // done, release SDL
  SDL_Quit();
  return 0;
}

void CEmuChip8::stop() {
  // cleanup

  // call destructor
  delete this;
}

void CEmuChip8::pause() {

}

void CEmuChip8::configure() {

}

CEmuChip8::CEmuChip8() :
 m_cpu(NULL), m_memory(NULL), m_input(NULL), m_display(NULL) {

}

CEmuChip8::~CEmuChip8() {
  SAFE_DELETE(m_cpu);
  SAFE_DELETE(m_memory);
  SAFE_DELETE(m_input);
  SAFE_DELETE(m_display);
}

extern "C" EXPORT IEmulator* get_emulator_interface() {
  return new CEmuChip8();
}
