//
// Created by valerino on 09/11/2017.
//

#ifndef PROJECT_CMEMORY_H
#define PROJECT_CMEMORY_H

#include <stdint.h>
#include <CConfiguration.h>

/*
 * docs taken from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */
#define CHIP8_MEMORY_SIZE 4 * 1024 // 4k size
#define CHIP8_START_ADDRESS 0x200
#define ETI660_START_ADDRESS 0x600
#define INTERPRETER_MAX_SIZE 0x1ff

class CMemory {
public:
  /**
   * initializes memory subsystem
   * @param cfg CConfiguration instance
   * @param rom_path path to the rom file
   * @throws std::system_error(ENOENT) when not found
   */
  CMemory(CConfiguration* cfg, const char *rom_path);

  /**
   * get 16 bit from memory, big endian
   * @param address vm address
   * @return
   */
  uint16_t get_word(uint16_t address);

  /**
   * put 16 bit to memory, big endian
   * @param address vm address
   * @param w the word
   */
  void put_word(uint16_t address, uint16_t w);

  /**
   * get 8 bit from memory
   * @param address vm address
   * @return
   */
  uint8_t get_byte(uint16_t address);

  /**
   * put 8 bit to memory
   * @param address vm address
   * @param b the byte
   */
  void put_byte(uint16_t address, uint8_t b);

  /**
   * get array of bytes from memory
   * @param address vm address
   * @param size size to fetch
   * @return
   */
  std::vector<uint8_t> get_bytes(uint16_t address, uint16_t size);

  /**
   * put array of bytes to memory
   * @param address vm address
   * @param b the array
   */
  void put_bytes(uint16_t address, std::vector<uint8_t> b);

  /**
   * returns start offset of the super-chip8 charset
   * @return
   */
  uint16_t schip8_charset_offset();

  /**
   * access to underlying buffer
   * @return
   */
  uint8_t *cmem() { return m_mem; }
  ~CMemory();

private:
  uint8_t *m_mem;
};

#endif // PROJECT_CMEMORY_H
