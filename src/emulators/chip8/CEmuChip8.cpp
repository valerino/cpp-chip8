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
  return std::string("vue-chip8");
}

std::string CEmuChip8::version() {
  return std::string("v1.0");
}

std::string CEmuChip8::info() {
  return std::string("Chip8/Super-Chip8 emulator");
}

std::string CEmuChip8::options() {
  return std::string("");
}

int CEmuChip8::start(const char *rom_path) {
  // load configuration
  CConfiguration cfg("emulators/chip8/chip8.json");
  cfg.read("{ \"mode\": 0, \"instructions_per_cycle\": 12}");
  int mode = cfg.get<int>("mode");
  int instructions_per_cycle = cfg.get<int>("instructions_per_cycle");
  CDbg::notify("chip8->mode=%d", mode);
  CDbg::notify("chip8->instructions_per_cycle=%d", instructions_per_cycle);

  // initialize SDL
  int res = SDL_Init(SDL_INIT_EVERYTHING);
  if (res != 0) {
    CDbg::error("%s", SDL_GetError());
    return 1;
  }
  CDbg::notify("SDL initialized");

  // initialize subsystems
  std::string charset_path("emulators/chip8/chip8_charset.bin");
  try {
    m_memory = new CMemory(mode, charset_path.c_str(), rom_path);
  } catch (std::system_error &e) {
    CDbg::error(e.what());
    SDL_Quit();
    return 1;
  }
  m_display = new CDisplay(mode, m_memory);
  m_input = new CInput();
  m_sound = new CSound();
  m_cpu = new CCpu(mode, m_memory, m_input, m_display, m_sound);

  // set running
  m_running = true;

  // loop
  while (m_running) {
    uint32_t ticks_start_ms = SDL_GetTicks();
    for (int i=0;i<instructions_per_cycle;i++) {
      res = m_cpu->step();
      if (res == ERROR_EXIT_OPCODE || res == ERROR_INVALID_OPCODE) {
        // EXIT instruction found or invalid opcode
        m_running = false;
        break;
      }
    }
    if (m_running == false) {
      break;
    }

    if (m_cpu->update_display()) {
      m_display->update();
    }

    // update timers
    m_cpu->updateTimers();

    // cap at 60hz
    int ticks_at_60hz_ms = 1000/60;
    uint32_t ticks_end_ms = SDL_GetTicks();
    uint32_t elapsed = ticks_end_ms - ticks_start_ms;
    if (elapsed < (ticks_at_60hz_ms)) {
      SDL_Delay(ticks_at_60hz_ms - elapsed);
    }

    // poll sdl events
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        // window closed
        m_running = false;
      }
    }
  }

  // done, release SDL
  SDL_Quit();
  return 0;
}

void CEmuChip8::stop() {
  // cleanup
  m_running = false;
}

void CEmuChip8::pause() { m_paused = !m_paused; }

void CEmuChip8::configure() {}

CEmuChip8::CEmuChip8()
    : m_cpu(NULL), m_memory(NULL), m_input(NULL), m_display(NULL) {}

CEmuChip8::~CEmuChip8() {
  SAFE_DELETE(m_cpu);
  SAFE_DELETE(m_memory);
  SAFE_DELETE(m_input);
  SAFE_DELETE(m_display);
  SAFE_DELETE(m_sound);
}

extern "C" EXPORT IEmulator *get_emulator_interface() {
  return new CEmuChip8();
}
