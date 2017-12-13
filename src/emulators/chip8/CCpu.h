//
// Created by valerino on 09/11/2017.
//

#ifndef PROJECT_CCPU_H
#define PROJECT_CCPU_H

#include "CMemory.h"
#include "CInput.h"
#include "CDisplay.h"
#include "CSound.h"
#include <CConfiguration.h>
/*
 * cpu implemented using documentation found here:
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM (chip8 only)
 * https://github.com/trapexit/chip-8_documentation/blob/master/Misc/SCHIP-8%20v1.1.txt
 * http://www.pong-story.com/chip8/ (download the chip8 emulator, docs are in the .zip)
 */

#define ERROR_INVALID_OPCODE -2
#define ERROR_EXIT_OPCODE -1

// implements chip8/super-chip8 vm
class CCpu {
public:
  /**
   * initializes the cpu
   * @param mem represents emulated memory
   * @param display represents emulated display
   * @param input represents emulated input
   * @param sound represents emulated sound hw
   */
  CCpu(CMemory *mem, CDisplay *display, CInput *input, CSound *sound);
  ~CCpu();

  /**
   * step an instruction
   * @return > 0 : PC must increment by the return value. either, returns
   * ERROR_INVALID_OPCODE or ERROR_EXIT_OPCODE
   */
  int step();

  /**
   * returns wether the display must be updated, must be called after step()
   * @return
   */
  bool update_display();

  /**
   * returns MODE_CHIP8 or MODE_SUPERCHIP8, depending on the current cpu mode
   * @return
   */
  uint8_t mode() { return m_mode; }

  /**
   * update the delay and sound timers
   */
  void updateTimers();

private:
  int m_mode;
  CMemory *m_mem;
  CInput *m_input;
  CDisplay *m_display;
  CSound *m_sound;
  uint16_t m_I;                     // I register
  std::array<uint8_t, 16> m_V;      // V0-VF
  std::array<uint8_t, 16> m_flags;  // RPL user flags (super-chip8 only)
  uint8_t m_T;                      // timer register
  uint8_t m_D;                      // delay register
  uint16_t m_PC;                    // PC register
  std::array<uint16_t, 16> m_stack; // the stack
  uint8_t m_SP; // points to top of m_stack

  /* will be true when cpu instruction causes the display to be updated in the next cycle */
  bool m_update_display;

  /* cpu fixes */
  bool m_fix_fx55_fx65;
  bool m_fix_8xy6_8xye;
  bool m_disable_auto_schip8_cpu_fixes;

  /* decoders */
  typedef int (CCpu::*DecoderFunc)(uint16_t);
  std::map<int, DecoderFunc> m_decoders;
  int decode_0(uint16_t addr);
  int decode_1(uint16_t addr);
  int decode_2(uint16_t addr);
  int decode_3(uint16_t addr);
  int decode_4(uint16_t addr);
  int decode_5(uint16_t addr);
  int decode_6(uint16_t addr);
  int decode_7(uint16_t addr);
  int decode_8(uint16_t addr);
  int decode_9(uint16_t addr);
  int decode_A(uint16_t addr);
  int decode_B(uint16_t addr);
  int decode_C(uint16_t addr);
  int decode_D(uint16_t addr);
  int decode_E(uint16_t addr);
  int decode_F(uint16_t addr);
};

#endif // PROJECT_CCPU_H
