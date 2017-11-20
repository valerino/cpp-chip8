//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CDisplay.h"
#include "CInput.h"
#include "CCpu.h"
#include "CEmuChip8.h"

std::string CEmuChip8::name() { return std::string("vue-chip8"); }

std::string CEmuChip8::version() { return std::string("v1.0"); }

std::string CEmuChip8::info() {
  return std::string("Chip8/Super-Chip8 emulator");
}

std::string CEmuChip8::options() { return std::string(""); }

void CEmuChip8::stop() { m_running = false; }

void CEmuChip8::pause() { m_paused = !m_paused; }

void CEmuChip8::configure() {}

void CEmuChip8::poll() {
  // poll sdl events
  uint8_t *keys;
  if (CSDLUtils::SDLPollEvents(&keys) != 0) {
    // must quit
    m_running = false;
  }
  if (keys) {
    if (keys[SDL_SCANCODE_ESCAPE]) {
      // abort emulation
      m_running = false;
    } else if (keys[SDL_SCANCODE_P]) {
      // pause/unpause emulation
      pause();
    }
  }
}

int CEmuChip8::start(const char *rom_path) {
  // load configuration
  CConfiguration cfg("emulators/chip8/chip8.json");

  // provide a default
  cfg.read("{ \"mode\": \"chip8\","
           "\"instructions_per_cycle\": 12,"
           "\"scale\": 10.0,"
           "\"fullscreen\": true,"
           "\"draw_color\": \"white\"}");
  std::string modestring = cfg.get<std::string>("mode");
  double scale_factor = cfg.get<double>("scale");
  bool full_screen = cfg.get<bool>("full_screen");
  int instructions_per_cycle = cfg.get<int>("instructions_per_cycle");
  std::string draw_color = cfg.get<std::string>("draw_color");
  CDbg::notify("chip8->mode=%s", modestring.data());
  CDbg::notify("chip8->instructions_per_cycle=%d", instructions_per_cycle);
  CDbg::notify("chip8->full_screen=%d", full_screen);
  CDbg::notify("chip8->scale=%f", scale_factor);
  CDbg::notify("chip8->draw_color=%s", draw_color.data());

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
    m_memory = new CMemory(&cfg, charset_path.c_str(), rom_path);
  } catch (std::system_error &e) {
    CDbg::error(e.what());
    SDL_Quit();
    return 1;
  }

  // set running
  m_running = true;

  m_display = new CDisplay(&cfg, m_memory);
  m_input = new CInput();
  m_sound = new CSound();
  m_cpu = new CCpu(&cfg, m_memory, m_display, m_input, m_sound);

  // loop
  while (m_running) {
    if (m_paused) {
      // just delay....
      poll();
      continue;
    }

    uint32_t ticks_start_ms = SDL_GetTicks();
    for (int i = 0; i < instructions_per_cycle; i++) {
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

    // update timers
    m_cpu->updateTimers();

    if (m_cpu->update_display()) {
      m_display->update();
    }

    // cap at 60hz
    int ticks_at_60hz_ms = 1000 / 60;
    uint32_t ticks_end_ms = SDL_GetTicks();
    uint32_t elapsed = ticks_end_ms - ticks_start_ms;
    if (elapsed < (ticks_at_60hz_ms)) {
      // sleep for the remaining time to reach 60hz
      SDL_Delay(ticks_at_60hz_ms - elapsed);
    }

    // poll sdl events
    poll();
  }

  // done, release SDL
  SDL_Quit();
  return 0;
}

CEmuChip8::CEmuChip8()
    : m_cpu(NULL), m_memory(NULL), m_input(NULL), m_display(NULL),
      m_sound(NULL) {
  // CDbg::set_debug_level(DBG_NONE);
}

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
