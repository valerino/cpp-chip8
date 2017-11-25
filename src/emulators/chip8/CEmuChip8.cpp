//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CDisplay.h"
#include "CInput.h"
#include "CCpu.h"
#include "defs.h"
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

void CEmuChip8::sdl_poll() {
  // sdl_poll sdl events
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
           "\"scale\": 10.0,"
           "\"fullscreen\": true,"
           "\"draw_color\": \"white\"}");

  // get operation mode
  int mode;
  std::string mode_string = cfg.get<std::string>("mode");
  if (mode_string == "chip8") {
    mode = MODE_CHIP8;
  }
  else if (mode_string == "sc8") {
    mode = MODE_SUPERCHIP8;
  }

  // print configuration
  double scale_factor = cfg.get<double>("scale");
  bool full_screen = cfg.get<bool>("full_screen");
  std::string draw_color = cfg.get<std::string>("draw_color");
  CDbg::notify("mode=%s,full_screen=%d,scale=%f,draw_color=%s", mode_string.data(), full_screen, scale_factor, draw_color.data());

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
    CUIUtils::show_toast_message(MSG_ERROR, e.what());
    SDL_Quit();
    return 1;
  }

  // set running
  m_running = true;

  try {
    m_display = new CDisplay(&cfg, m_memory);
  }
  catch (std::exception e) {
    CDbg::error(e.what());
    SDL_Quit();
    return 1;
  }

  m_input = new CInput();
  m_sound = new CSound();
  m_cpu = new CCpu(&cfg, m_memory, m_display, m_input, m_sound);

  while (m_running) {
    // poll sdl events
    sdl_poll();

    // if the emulator is paused just delay loop unpaused
    if (m_paused) {
      continue;
    }

    // this is the virtual cpu clock, running at 500hz
    // (arbitrary, since it's not a real cpu ....)
    int clock_hz = 500;
    if (mode == MODE_SUPERCHIP8) {
      // on super chip8, cpu clock is at 1000hz
      clock_hz = 1000;
    }

    // a clock cycle is these many ms
    int clock_cycle_ms = 1000 / clock_hz;

    // delay and sound timers tick at 60hz
    int timers_hz = 60;

    // a timers cycle is these many ms
    int timers_cycle_ms = 1000 / timers_hz;

    // execute a cycle
    int instructions = 0;
    int elapsed_ms = 0;
    uint32_t start_ms = SDL_GetTicks();
    uint32_t end_ms = 0;
    for (;;) {
      // step the cpu, one instruction
      res = m_cpu->step();
      instructions++;
      if (res == ERROR_EXIT_OPCODE || res == ERROR_INVALID_OPCODE) {
        // EXIT instruction found or invalid opcode, next iteration will exit!
        m_running = false;
        break;
      }

      // update display if needed
      if (m_cpu->update_display()) {
        m_display->update();
      }

      // check clock hz, if we have run for the number of ms in a clock cycle
      // we exit the execution loop
      end_ms = SDL_GetTicks();
      elapsed_ms = (end_ms - start_ms);
      if (elapsed_ms >= clock_cycle_ms) {
        break;
      }
    }

    // cap at 60hz
    end_ms = SDL_GetTicks();
    elapsed_ms += (end_ms - start_ms);
    if (elapsed_ms < timers_cycle_ms) {
      // sleep for the remaining to complete the 60hz cap
      int remaining = timers_cycle_ms - elapsed_ms;
      SDL_Delay(remaining);
    }

    // at 60hz, we have to update timers too
    m_cpu->updateTimers();
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
