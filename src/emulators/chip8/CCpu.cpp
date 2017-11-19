//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include "CCpu.h"
#include "defs.h"

CCpu::~CCpu() {}

int CCpu::step() {
  // fetch instruction at pc
  uint16_t w = m_mem->get_word(m_PC);
  CDbg::verbose("PC=%x I=%x (opcode=%x) ==>", m_PC, m_I, w);

  // get opcode category and address
  uint8_t opcode_class = w >> 12;
  uint16_t addr = w & 0x0fff;

  // and call the appropriate opcode decoder with the interesting 12 bits
  DecoderFunc f = m_decoders[opcode_class];
  int res = (this->*f)(addr);
  if (res == ERROR_INVALID_OPCODE) {
    // will halt
    CDbg::error("INVALID OPCODE: 0x%x", w);
  } else if (res == ERROR_EXIT_OPCODE) {
    CDbg::warning("EXIT encountered!");
  } else {
    // increment PC
    m_PC += res;
  }
  return res;
}

CCpu::CCpu(int mode, CMemory *mem, CInput *input, CDisplay *display,
           CSound *sound)
    : m_mem{mem}, m_input{input},
      m_sound(sound), m_display{display}, m_mode{mode}, m_I{0}, m_V{}, m_T{0},
      m_D{0}, m_PC{CHIP8_START_ADDRESS}, m_stack{}, m_flags{}, m_SP{0} {
  if (m_mode == MODE_ETI660) {
    // different start address
    m_PC = ETI660_START_ADDRESS;
  }

  // initializes the random number generator
  srand(time(NULL));

  // map decoders for each opcode category
  m_decoders = {
      {0, &CCpu::decode_0},   {1, &CCpu::decode_1},   {2, &CCpu::decode_2},
      {3, &CCpu::decode_3},   {4, &CCpu::decode_4},   {5, &CCpu::decode_5},
      {6, &CCpu::decode_6},   {7, &CCpu::decode_7},   {8, &CCpu::decode_8},
      {9, &CCpu::decode_9},   {0xa, &CCpu::decode_A}, {0xb, &CCpu::decode_B},
      {0xc, &CCpu::decode_C}, {0xd, &CCpu::decode_D}, {0xe, &CCpu::decode_E},
      {0xf, &CCpu::decode_F},
  };
}

int CCpu::decode_0(uint16_t addr) {
  // default increment PC on return
  int res = sizeof(uint16_t);

  // check if it's the scroll down opcode
  if ((addr & 0x0f0) >> 4 == 0xc) {
    /*
     * 00CN*    Scroll display N lines down (super chip8)
     */
    uint8_t n = addr & 0x00f;
    // scroll down display
    CDbg::verbose("SCD %d", n);
    m_display->scroll_down(n);
    m_update_display = true;
    return res;
  }
  if ((addr & 0xf00) >> 4 != 0) {
    /*
      0nnn - SYS addr
      Jump to a machine code routine at nnn.

      This instruction is only used on the old computers on which Chip-8 was
      originally implemented. It is ignored by modern interpreters.
     */
    m_PC = addr;
    return 0;
  }

  uint8_t kk = addr & 0x0ff;
  switch (kk) {
  case 0xe0:
    /*
      00E0 - CLS
      Clear the display.
     */
    CDbg::verbose("CLS");
    m_display->clear();
    m_update_display = true;
    break;

  case 0xee:
    /*
    00EE - RET
    Return from a subroutine

    The interpreter sets the program counter to the address at the top of the
    stack, then subtracts 1 from the stack pointer.
    */
    CDbg::verbose("RET");
    m_PC = m_stack[m_SP];
    m_SP--;
    break;

  case 0xfb:
    /*
     * 00FB*    Scroll display 4 pixels right (super chip8)
     */
    CDbg::verbose("SCR");
    m_display->scroll_right();
    m_update_display = true;
    break;

  case 0xfc:
    /*
     * 00FC*    Scroll display 4 pixels left (super chip8)
     */
    CDbg::verbose("SCL");
    m_display->scroll_left();
    m_update_display = true;
    break;

  case 0xfd:
    /*
     * 00FD*    Exit CHIP interpreter (super chip8)
     */
    CDbg::verbose("EXIT");
    res = -2;
    break;

  case 0xfe:
    /*
     * 00FE*    Disable extended screen mode (super chip8)
     */
    CDbg::verbose("LOW");
    m_display->set_mode(MODE_CHIP8);
    m_update_display = true;
    break;

  case 0xff:
    /*
     * 00FF*    Enable extended screen mode for full-screen graphics (super chip8)
     */
    CDbg::verbose("HIGH");
    m_display->set_mode(MODE_SUPER_CHIP8);
    m_update_display = true;
    break;

  default:
    // unknown opcode
    res = ERROR_INVALID_OPCODE;
    break;
  }

  return res;
}

int CCpu::decode_1(uint16_t addr) {
  /*
    1nnn - JP addr
    Jump to location nnn.

    The interpreter sets the program counter to nnn.
   */
  CDbg::verbose("JP %x", addr);
  m_PC = addr;
  return 0;
}

int CCpu::decode_2(uint16_t addr) {
  /*
  2nnn - CALL addr
  Call subroutine at nnn
  The interpreter increments the stack pointer, then puts the current PC on the
  top of the stack. The PC is then set to nnn.
  */
  CDbg::verbose("CALL %x", addr);
  m_SP++;
  m_stack[m_SP] = m_PC;
  m_PC = addr;
  return 0;
}

int CCpu::decode_3(uint16_t addr) {
  /*
    3xkk - SE Vx, byte
    Skip next instruction if Vx = kk.

    The interpreter compares register Vx to kk, and if they are equal,
    increments the program counter by 2.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = addr & 0xff;
  CDbg::verbose("SE V%x, %x", x, kk);
  if (m_V[x] == kk) {
    // skip next instruction
    m_PC += sizeof(uint16_t);
  }
  return sizeof(uint16_t);
}

int CCpu::decode_4(uint16_t addr) {
  /*
    4xkk - SNE Vx, byte
    Skip next instruction if Vx != kk.

    The interpreter compares register Vx to kk, and if they are not equal,
    increments the program counter by 2.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = addr & 0xff;
  CDbg::verbose("SNE V%x, %x", x, kk);
  if (m_V[x] != kk) {
    // skip next instruction
    m_PC += sizeof(uint16_t);
  }
  return sizeof(uint16_t);
}

int CCpu::decode_5(uint16_t addr) {
  /*
    5xy0 - SE Vx, Vy
    Skip next instruction if Vx = Vy.

    The interpreter compares register Vx to register Vy, and if they are equal,
    increments the program counter by 2.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t y = (addr & 0x0f0) >> 4;
  CDbg::verbose("SE V%x, V%x", x, y);
  if (m_V[x] == m_V[y]) {
    // skip next instruction
    m_PC += sizeof(uint16_t);
  }
  return sizeof(uint16_t);
}

int CCpu::decode_6(uint16_t addr) {
  /*
    6xkk - LD Vx, byte
    Set Vx = kk.

    The interpreter puts the value kk into register Vx.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = addr & 0xff;
  CDbg::verbose("LD V%x, %x", x, kk);
  m_V[x] = kk;
  return sizeof(uint16_t);
}

int CCpu::decode_7(uint16_t addr) {
  /*
    7xkk - ADD Vx, byte
    Set Vx = Vx + kk.

    Adds the value kk to the value of register Vx, then stores the result in Vx.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = addr & 0xff;
  CDbg::verbose("ADD V%x, %x", x, kk);
  m_V[x] += kk;
  return sizeof(uint16_t);
}

int CCpu::decode_8(uint16_t addr) {
  /*
    8xy0 - LD Vx, Vy
    Set Vx = Vy.

    Stores the value of register Vy in register Vx.
   */
  uint8_t n = addr & 0xf;
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t y = (addr & 0x0f0) >> 4;
  int res = sizeof(uint16_t);
  uint32_t r = 0;

  switch (n) {
  case 0:
    /*
      8xy0 - LD Vx, Vy
      Set Vx = Vy.

      Stores the value of register Vy in register Vx.
     */
    CDbg::verbose("LD V%x, V%x", x, y);
    m_V[x] = m_V[y];
    break;
  case 1:
    /*
      8xy1 - OR Vx, Vy
      Set Vx = Vx OR Vy.

      Performs a bitwise OR on the values of Vx and Vy, then stores the result
      in Vx. A bitwise OR compares the corrseponding bits from two values, and
      if either bit is 1, then the same bit in the result is also 1. Otherwise,
      it is 0.
     */
    CDbg::verbose("OR V%x, V%x", x, y);
    m_V[x] |= m_V[y];
    break;
  case 2:
    /*
      8xy2 - AND Vx, Vy
      Set Vx = Vx AND Vy.

      Performs a bitwise AND on the values of Vx and Vy, then stores the result
      in Vx. A bitwise AND compares the corrseponding bits from two values, and
      if both bits are 1, then the same bit in the result is also 1. Otherwise,
      it is 0.
     */
    CDbg::verbose("AND V%x, V%x", x, y);
    m_V[x] &= m_V[y];
    break;
  case 3:
    /*
      8xy3 - XOR Vx, Vy
      Set Vx = Vx XOR Vy.

      Performs a bitwise exclusive OR on the values of Vx and Vy, then stores
      the result in Vx. An exclusive OR compares the corrseponding bits from two
      values, and if the bits are not both the same, then the corresponding bit
      in the result is set to 1. Otherwise, it is 0.
     */
    CDbg::verbose("XOR V%x, V%x", x, y);
    m_V[x] ^= m_V[y];
    break;
  case 4:
    /*
      8xy4 - ADD Vx, Vy
      Set Vx = Vx + Vy, set VF = carry
      The values of Vx and Vy are added together. If the result is greater than
      8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits
      of the result are kept, and stored in Vx.
    */
    CDbg::verbose("ADD V%x, V%x", x, y);
    r = m_V[x] + m_V[y];
    if (r > (0xff - r)) {
      // set carry flag
      m_V[0xf] = 1;
    } else {
      // clear carry flag
      m_V[0xf] = 0;
    }

    // get lowest 8 bits
    m_V[x] = r & 0xff;
    break;
  case 5:
    /*
      8xy5 - SUB Vx, Vy
      Set Vx = Vx - Vy, set VF = NOT borrow
      If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from
      Vx, and the results stored in Vx.
     */
    CDbg::verbose("SUB V%x, V%x", x, y);
    if (m_V[x] > m_V[y]) {
      // set carry flag
      m_V[0xf] = 1;
    } else {
      // clear carry flag
      m_V[0xf] = 0;
    }
    m_V[x] = m_V[x] - m_V[y];
    break;
  case 6:
    /*
      8xy6 - SHR Vx {, Vy}
      Set Vx = Vx SHR 1.

      If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
      Then Vx is divided by 2.
    */
    CDbg::verbose("SHR V%x, 1", x);
    if (m_V[x] & 1) {
      // LSB is set, set carry flag
      m_V[0xf] = 1;
    } else {
      // clear carry flag
      m_V[0xf] = 0;
    }
    m_V[x] = m_V[x] >> 1;
    break;
  case 7:
    /*
      8xy7 - SUBN Vx, Vy
      Set Vx = Vy - Vx, set VF = NOT borrow
      If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from
      Vy, and the results stored in Vx.
    */
    CDbg::verbose("SUBN V%x, V%x", x, y);
    if (m_V[y] > m_V[x]) {
      // set carry flag
      m_V[0xf] = 1;
    } else {
      // clear carry flag
      m_V[0xf] = 0;
    }
    m_V[x] = m_V[y] - m_V[x];
    break;
  case 0xe:
    /*
      8xyE - SHL Vx {, Vy}
      Set Vx = Vx SHL 1.

      If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to
      0. Then Vx is multiplied by 2.
     */
    CDbg::verbose("SHL V%x, 1", x);
    if (m_mode == MODE_SUPER_CHIP8) {
      // seems super-chip8 requires the v[x] register to be shifted!
      y = x;
    }
    if ((m_V[y] << 7) & 1) {
      // MSB is set, set carry flag
      m_V[0xf] = 1;
    } else {
      // clear carry flag
      m_V[0xf] = 0;
    }
    m_V[x] = m_V[y] << 1;
    break;

  default:
    // unknown
    res = ERROR_INVALID_OPCODE;
    break;
  }
  return res;
}

int CCpu::decode_9(uint16_t addr) {
  /*
    9xy0 - SNE Vx, Vy
    Skip next instruction if Vx != Vy.

    The values of Vx and Vy are compared, and if they are not equal, the program
    counter is increased by 2.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t y = (addr & 0x0f0) >> 4;
  CDbg::verbose("SNE V%x, V%x", x, y);
  if (m_V[x] != m_V[y]) {
    // skip next instruction
    m_PC += sizeof(uint16_t);
  }
  return sizeof(uint16_t);
}

int CCpu::decode_A(uint16_t addr) {
  /*
    Annn - LD I, addr
    Set I = nnn.

    The value of register I is set to nnn.
   */
  CDbg::verbose("LDI I, %x", addr);
  m_I = addr;
  return sizeof(uint16_t);
}

int CCpu::decode_B(uint16_t addr) {
  /*
    Bnnn - JP V0, addr
    Jump to location nnn + V0.

    The program counter is set to nnn plus the value of V0.
   */
  CDbg::verbose("JP V0, %x", addr);
  m_PC = m_V[0] + addr;
  return 0;
}

int CCpu::decode_C(uint16_t addr) {
  /*
    Cxkk - RND Vx, byte
    Set Vx = random byte AND kk.

    The interpreter generates a random number from 0 to 255, which is then ANDed
    with the value kk. The results are stored in Vx. See instruction 8xy2 for
    more information on AND.
   */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = addr & 0x0ff;
  CDbg::verbose("RND V%x, %x", x, kk);
  m_V[x] = (rand() % 0xff) & kk;
  return sizeof(uint16_t);
}

int CCpu::decode_D(uint16_t addr) {
  /*
    Dxyn - DRW Vx, Vy, nibble
    Display n-byte sprite starting at memory location I at (Vx, Vy), set VF =
    collision.

    The interpreter reads n bytes from memory, starting at the address stored in
    I. These bytes are then displayed as sprites on screen at coordinates (Vx,
    Vy). Sprites are XORed onto the existing screen. If this causes any pixels
    to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is
    positioned so part of it is outside the coordinates of the display, it wraps
    around to the opposite side of the screen. See instruction 8xy3 for more
    information on XOR, and section 2.4, Display, for more information on the
    Chip-8 screen and sprites.
  */
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t y = (addr & 0x0f0) >> 4;
  uint8_t n = addr & 0x00f;
  CDbg::verbose("DRW V%x, V%x, %x", x, y, n);

  // read memory at I
  std::vector<uint8_t> sprite = m_mem->get_bytes(m_I, n);

  // draw
  int collision =
      m_display->draw_sprite(sprite.data(), sprite.size(), m_V[x], m_V[y]);
  if (collision) {
    // set flag
    m_V[0xf] = 1;
  } else {
    // unset flag
    m_V[0xf] = 0;
  }
  m_update_display = true;
  return sizeof(uint16_t);
}

int CCpu::decode_E(uint16_t addr) {
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = addr & 0x0ff;
  int res = sizeof(uint16_t);
  switch (kk) {
  case 0x9e:
    /*
      Ex9E - SKP Vx
      Skip next instruction if key with the value of Vx is pressed.

      Checks the keyboard, and if the key corresponding to the value of Vx is
      currently in the down position, PC is increased by 2.
     */
    CDbg::verbose("SKP V%x", x);
    // check if key is pressed
    if (m_input->is_key_pressed(m_V[x])) {
      // skip next instruction
      m_PC += sizeof(uint16_t);
    }
    break;
  case 0xa1:
    /*
      ExA1 - SKNP Vx
      Skip next instruction if key with the value of Vx is not pressed.

      Checks the keyboard, and if the key corresponding to the value of Vx is
      currently in the up position, PC is increased by 2.
     */
    CDbg::verbose("SKNP V%x", x);
    // check if key is NOT pressed
    if (!m_input->is_key_pressed(m_V[x])) {
      // skip next instruction
      m_PC += sizeof(uint16_t);
    }
    break;
  default:
    res = ERROR_INVALID_OPCODE;
    break;
  }
  return res;
}

int CCpu::decode_F(uint16_t addr) {
  uint8_t x = (addr & 0xf00) >> 8;
  uint8_t kk = (addr & 0x0ff);
  int res = sizeof(uint16_t);
  switch (kk) {
  case 0x07:
    /*
      Fx07 - LD Vx, DT
      Set Vx = delay timer value.

      The value of DT is placed into Vx.
     */
    CDbg::verbose("LD V%x, Dt", x);
    m_V[x] = m_D;
    break;
  case 0x0a:
    /*
      Fx0A - LD Vx, K
      Wait for a key press, store the value of the key in Vx.

      All execution stops until a key is pressed, then the value of that key is
      stored in Vx.
     */
    CDbg::verbose("LD V%x, K", x);
    m_V[x] = m_input->wait_key();
    break;
  case 0x15:
    /*
      Fx15 - LD DT, Vx
      Set delay timer = Vx.

      DT is set equal to the value of Vx.
    */
    CDbg::verbose("LD Dt, V%x", x);
    m_D = m_V[x];
    break;
  case 0x18:
    /*
      Fx18 - LD ST, Vx
      Set sound timer = Vx.

      ST is set equal to the value of Vx.
    */
    CDbg::verbose("LD St, V%x", x);
    m_T = m_V[x];
    break;
  case 0x1e:
    /*
      Fx1E - ADD I, Vx
      Set I = I + Vx.

      The values of I and Vx are added, and the results are stored in I.
     */
    CDbg::verbose("ADD I, V%x", x);
    m_I += m_V[x];
    break;
  case 0x29:
    /*
      Fx29 - LD F, Vx
      Set I = location of sprite for digit Vx.

      The value of I is set to the location for the hexadecimal sprite
      corresponding to the value of Vx. See section 2.4, Display, for more
      information on the Chip-8 hexadecimal font.
     */
    CDbg::verbose("LD F, V%x", x);
    // get font sprite at v[x], each sprite is 5 byte, charset starts at 0
    m_I = m_V[x] * 5;
    break;
  case 0x30:
    /*
     * FX30*    Point I to 10-byte font sprite for digit VX (0..9) (super chip8)
     */
    CDbg::verbose("LD FX, V%x", x);
    // extended istruction, get font sprite at v[x], each sprite is 10 byte,
    // charset starts at 0
    m_I = m_V[x] * 10;
    break;
  case 0x33:
    /*
      Fx33 - LD B, Vx
      Store BCD representation of Vx in memory locations I, I+1, and I+2.

      The interpreter takes the decimal value of Vx, and places the hundreds
      digit in memory at location in I, the tens digit at location I+1, and the
      ones digit at location I+2.
     */
    CDbg::verbose("LD B, V%x", x);
    // store in BCD format, first hundreds
    m_mem->put_byte(m_I, m_V[x] / 100);
    // then tens
    m_mem->put_byte(m_I + 1, (m_V[x] % 100) / 10);
    // then units
    m_mem->put_byte(m_I + 2, (m_V[x] % 100) % 10);
    break;
  case 0x55:
    /*
      Fx55 - LD [I], Vx
      Store registers V0 through Vx in memory starting at location I.

      The interpreter copies the values of registers V0 through Vx into memory,
      starting at the address in I.
     */
    CDbg::verbose("LD [I], V%x", x);
    for (int i = 0; i <= x; i++) {
      m_mem->put_byte(m_I + i, m_V[i]);
    }
    break;
  case 0x65:
    /*
      Fx65 - LD Vx, [I]
      Read registers V0 through Vx from memory starting at location I.

      The interpreter reads values from memory starting at location I into
      registers V0 through Vx.
     */
    CDbg::verbose("LD V%x, [I]", x);
    for (int i = 0; i <= x; i++) {
      m_V[i] = m_mem->get_byte(m_I + i);
    }
    break;
  case 0x75:
    /*
     * FX75*    Store V0..VX in RPL user flags (X <= 7) (super chip8)
     */
    CDbg::verbose("LD V%x, [RPL]", x);
    for (int i = 0; i < x; i++) {
      m_flags[i] = m_V[i];
    }
    break;
  case 0x85:
    /*
     * FX85*    Read V0..VX from RPL user flags (X <= 7) (super chip8)
     */
    CDbg::verbose("LD [RPL], V%x", x);
    for (int i = 0; i < x; i++) {
      m_V[i] = m_flags[i];
    }
    break;

  default:
    res = ERROR_INVALID_OPCODE;
    break;
  }
  return res;
}

void CCpu::updateTimers() {
  // update delay timer
  if (m_D != 0) {
    m_D--;
  }

  // update sound timer
  if (m_T != 0) {
    m_sound->beep();
    m_T--;
  }
}

bool CCpu::update_display() {
  bool b = m_update_display;
  if (m_update_display) {
    m_update_display = false;
  }
  return b;
}