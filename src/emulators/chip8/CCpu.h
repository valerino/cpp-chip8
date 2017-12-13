/**
 * @file   CCpu.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip8 CPU (a VM, in reality....)
 */
#ifndef PROJECT_CCPU_H
#define PROJECT_CCPU_H

#include "CMemory.h"
#include "CInput.h"
#include "CDisplay.h"
#include "CSound.h"
#include <CConfiguration.h>

/** @brief returned by step() on invalid opcode */
#define ERROR_INVALID_OPCODE -2

/** @brief returned by step() when EXIT opcode is executed by the cpu */
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

  /**
   * destructor
   */
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
   * update the delay and sound timers (both must tick at 60hz)
   */
  void updateTimers();

private:
  int m_mode;
  CMemory *m_mem;
  CInput *m_input;
  CDisplay *m_display;
  CSound *m_sound;
  bool m_update_display;
  bool m_fix_fx55_fx65;
  bool m_fix_8xy6_8xye;
  bool m_disable_auto_schip8_cpu_fixes;
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

  /** @brief I address register */
  uint16_t m_I;
  /** @brief 16 8bit registers, register VF is the carry flag */
  std::array<uint8_t, 16> m_V;
  /** @brief super-chip8 only, 16 8bit flags affected by the FX75, FX85 instructions */
  std::array<uint8_t, 16> m_flags;
  /** @brief timer register, must tick at 60hz */
  uint8_t m_T;
  /** @brief delay register, must tick at 60hz */
  uint8_t m_D;
  /** @brief the program counter register */
  uint16_t m_PC;
  /** @brief 16 positions 16bit stack */
  std::array<uint16_t, 16> m_stack; // the stack
  /** @brief stack register, points to the top of stack */
  uint8_t m_SP;
};

#endif // PROJECT_CCPU_H
