/**
 * @file   CEmuChip8.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip8 emulator
 */

#include <vuelib.h>
#include "CDisplay.h"
#include "CInput.h"
#include "CCpu.h"
#include "defs.h"
#include "CEmuChip8.h"

std::string CEmuChip8::name() { return std::string(EMULATOR_NAME); }

std::string CEmuChip8::version() { return std::string(EMULATOR_VERSION); }

std::string CEmuChip8::info() {
  return std::string("Chip8/Super-Chip8 emulator");
}

void CEmuChip8::stop() { m_running = false; }

void CEmuChip8::pause() { m_paused = !m_paused; }

void CEmuChip8::configure() {}

/**
 * poll SDL events and check if reserved keys (to exit and pause emulator) has been pressed
 */
void CEmuChip8::sdl_poll() {
  // poll system events with SDL
  uint8_t *keys;
  if (CSDLUtils::sdl_poll_events(&keys) != 0) {
    // must quit
    m_running = false;
    return;
  }

  // check if reserved keys (esc and p) are pressed
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
 * @param interval unused
 * @param param CCpu instance
 * @return
 */
static Uint32 timer_callback(Uint32 interval, void *param) {
  CCpu *cpu = (CCpu *) param;
  cpu->updateTimers();
  return interval;
}

int CEmuChip8::start(const char *rom_path) {
  // load configuration, providing a default
  std::string default_cfg = "{"
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
  CConfiguration::init("emulators/chip8/chip8.json", default_cfg.data());

  if (CConfiguration::instance()->get<bool>("dbg_verbose")) {
    // enable verbose debug prints
    CDbg::set_debug_level(DBG_VERBOSE);
  } else {
    // default debug level set to error
    CDbg::set_debug_level(DBG_ERROR);
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
  try {
    m_display = new CDisplay(m_memory);
    m_input = new CInput();
  }
  catch (std::exception e) {
    CUIUtils::show_toast_message(MSG_ERROR, e.what());
    SDL_Quit();
    return 1;
  }
  m_sound = new CSound();
  m_cpu = new CCpu(m_memory, m_display, m_input, m_sound);

  // setup 60hz delay and sound timers
  SDL_TimerID tid = SDL_AddTimer(1000 / 60, timer_callback, m_cpu);

  // target speed
  int hz=CConfiguration::instance()->get<int>("cpu_target_hz");

  // get start time
  uint32_t start_ms = SDL_GetTicks();

  // game loop
  m_running = true;
  while (m_running) {
    // poll sdl events
    sdl_poll();

    // if the emulator is paused, just loop
    if (m_paused) {
      continue;
    }

    // fetch an instruction
    res = m_cpu->step();
    if (res == ERROR_EXIT_OPCODE || res == ERROR_INVALID_OPCODE) {
      // EXIT instruction found or invalid opcode, next iteration will exit!
      m_running = false;
      break;
    }

    // update display if needed
    if (m_cpu->update_display()) {
      m_display->update();
    }

    // get end time
    uint32_t end_ms = SDL_GetTicks();

    if (hz == 0) {
      // use predefined target cpu speed
      if (m_cpu->mode() == MODE_CHIP8) {
        // super chip8 runs at about 500hz
        hz = 500;
      } else {
        // super chip8 runs at about 1000hz
        hz = 1000;
      }
    }

    // how many milliseconds for a cycle
    uint32_t cycle_ms = 1000/hz;

    // calculate difference, how long this cycle took
    uint32_t diff = end_ms - start_ms;
    if (diff < cycle_ms) {
      // cap at the cpu execution speed
      uint32_t sleep_time = cycle_ms - diff;
      SDL_Delay(sleep_time);
    }

    // update start time with the new time, and go on for the next cycle
    start_ms = end_ms;
  }

  // done, kill timer
  SDL_RemoveTimer(tid);

  // and release SDL
  SDL_Quit();
  return 0;
}

CEmuChip8::CEmuChip8()
    : m_cpu(NULL), m_memory(NULL), m_input(NULL), m_display(NULL),
      m_sound(NULL) {
}

CEmuChip8::~CEmuChip8() {
  SAFE_DELETE(m_cpu);
  SAFE_DELETE(m_memory);
  SAFE_DELETE(m_input);
  SAFE_DELETE(m_display);
  SAFE_DELETE(m_sound);
}

extern "C" EXPORT IEmulator *get_emulator_interface() {
  // returns the IEmulator interface to control this core
  return new CEmuChip8();
}
