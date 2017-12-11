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

/**
 * ticks at 60hz, updates sound and delay timer
 * @param interval
 * @param param
 * @return
 */
static Uint32 timer_callback(Uint32 interval, void *param) {
  CCpu *cpu = (CCpu *) param;
  cpu->updateTimers();
  return interval;
}

int CEmuChip8::start(const char *rom_path) {
  // load configuration, providing a default
  char* default_cfg="{"
      "    \"display_scale\": 10.0,\n"
      "    \"display_fullscreen\": false,\n"
      "    \"display_draw_color\": \"white\",\n"
      "    \"key_0\":\"0\",\n"
      "    \"key_1\":\"1\",\n"
      "    \"key_2\":\"2\",\n"
      "    \"key_3\":\"3\",\n"
      "    \"key_4\":\"4\",\n"
      "    \"key_5\":\"5\",\n"
      "    \"key_6\":\"6\",\n"
      "    \"key_7\":\"7\",\n"
      "    \"key_8\":\"8\",\n"
      "    \"key_9\":\"9\",\n"
      "    \"key_a\":\"a\",\n"
      "    \"key_b\":\"b\",\n"
      "    \"key_c\":\"c\",\n"
      "    \"key_d\":\"d\",\n"
      "    \"key_e\":\"e\",\n"
      "    \"key_f\":\"f\"\n"
      "}";
  CConfiguration::init("emulators/chip8/chip8.json", default_cfg);

  // disable debug prints
  if (CConfiguration::instance()->get<bool>("dbg_verbose")) {
    CDbg::set_debug_level(DBG_VERBOSE);
  }

  // initialize SDL
  int res = SDL_Init(SDL_INIT_EVERYTHING);
  if (res != 0) {
    CDbg::error("%s", SDL_GetError());
    return 1;
  }
  CDbg::notify("SDL initialized");

  // initialize subsystems
  try {
    m_memory = new CMemory(rom_path);
  } catch (std::system_error &e) {
    CUIUtils::show_toast_message(MSG_ERROR, e.what());
    SDL_Quit();
    return 1;
  }

  // set running
  m_running = true;

  try {
    m_display = new CDisplay(m_memory);
  }
  catch (std::exception e) {
    CDbg::error(e.what());
    SDL_Quit();
    return 1;
  }

  m_input = new CInput();
  m_sound = new CSound();
  m_cpu = new CCpu(m_memory, m_display, m_input, m_sound);

  // setup 60hz delay and sound timers
  SDL_TimerID tid = SDL_AddTimer(1000 / 60, timer_callback, m_cpu);

  while (m_running) {
    // poll sdl events
    sdl_poll();

    // if the emulator is paused just delay loop unpaused
    if (m_paused) {
      continue;
    }

    // override default speed ?
    int hz_override = CConfiguration::instance()->get<int>("speed_hz_override");
    int hz;
    if (hz_override) {
      // custom speed
      hz = hz_override;
    }
    else {
      // chip8 or super-chip8
      if (m_cpu->mode() == MODE_CHIP8) {
        // super chip8 runs at about 600hz
        hz = 500;
      }
      else {
        // super chip8 runs at about 1200hz
        hz = 1000;
      }
    }

    // execute a cycle
    float cycle_ms = (1.0 / hz) * 1000;
    uint32_t start_ms = SDL_GetTicks();
    uint32_t elapsed = 0;
    while (1) {
      res = m_cpu->step();
      if (res == ERROR_EXIT_OPCODE || res == ERROR_INVALID_OPCODE) {
        // EXIT instruction found or invalid opcode, next iteration will exit!
        m_running = false;
        break;
      }

      // cap at the desired hz, sleeping for the difference
      uint32_t end_ms = SDL_GetTicks();
      elapsed += (end_ms - start_ms);
      if (elapsed < cycle_ms) {
        SDL_Delay(cycle_ms - elapsed);
      }
      break;
    }

    // update display if needed
    if (m_cpu->update_display()) {
      m_display->update();
    }
  }

  // kill timer
  SDL_RemoveTimer(tid);

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
