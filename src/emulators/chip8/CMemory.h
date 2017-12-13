/**
 * @file   CMemory.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip-8 memory subsystem
 */
#ifndef PROJECT_CMEMORY_H
#define PROJECT_CMEMORY_H

#include <stdint.h>

/** @brief system memory size (4k) */
#define CHIP8_MEMORY_SIZE 4 * 1024

/** @brief start address for chip8/super-chip8 */
#define CHIP8_START_ADDRESS 0x200

/** @brief start address for ETI-660 mode */
#define ETI660_START_ADDRESS 0x600

class CMemory {
public:
  /**
   * initializes memory subsystem
   * @param rom_path path to the rom file
   * @throws std::system_error(ENOENT) when not found
   * @throws std::system_error(EINVAL) for invalid data (size mismatch)
   */
  CMemory(const char *rom_path);

  /**
   * get 16 bit from memory, big endian
   * @param address vm address
   * @return
   */
  uint16_t get_word(uint16_t address);

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
   * get path to the rom file the memory has been initialized with
   * @return
   */
  std::string path() { return m_path; }

  /**
   * returns start offset of the super-chip8 charset
   * @return
   */
  uint16_t schip8_charset_offset();

  /**
   * check if we're running in eti-660 mode (different start address)
   * @return
   */
  bool is_eti660() { return m_eti660; }

  /**
   * destructor
   */
  ~CMemory();

private:
  bool m_eti660;
  std::string m_path;
  uint8_t *m_mem;
};

#endif // PROJECT_CMEMORY_H
