//
// Created by valerino on 09/11/2017.
//

#include <vuelib.h>
#include <assert.h>
#include "CMemory.h"
#include "CFile.h"

CMemory::CMemory(CConfiguration *cfg, const char *charset_path,
                 const char *rom_path)
    : m_mem{NULL} {

  // allocate memory (4kb)
  m_mem = (uint8_t *)CMem::alloc(CHIP8_MEMORY_SIZE);

  // load charset
  uint32_t charset_size = 0;
  uint8_t *charset = (uint8_t *)CFile::to_buffer(charset_path, &charset_size);
  if (!charset) {
    // charset file not found/invalid, halt!
    CMem::free(charset);
    throw std::system_error(ENOENT, std::generic_category(),
                            std::string("can't find charset: ") +
                                std::string(charset_path));
  }
  assert(charset_size == CHARSET_SIZE);

  // load game rom
  uint32_t game_size;
  uint8_t *game = (uint8_t *)CFile::to_buffer(rom_path, &game_size);
  if (!game) {
    // game file not found, halt!
    CMem::free(charset);
    CMem::free(game);
    throw std::system_error(ENOENT, std::generic_category(),
                            std::string("can't find rom: ") +
                                std::string(rom_path));
  }
  assert(game_size <= (CHIP8_MEMORY_SIZE - INTERPRETER_MAX_SIZE));


  // copy charset at 0x0
  memcpy(m_mem, charset, CHARSET_SIZE);

  // copy game rom
  uint8_t *start_address =
      (cfg->get<std::string>("mode") == "eti660" ? m_mem + ETI660_START_ADDRESS
                                                 : m_mem + CHIP8_START_ADDRESS);
  memcpy(start_address, game, game_size);

  CMem::free(game);
  CMem::free(charset);
}

CMemory::~CMemory() { CMem::free(m_mem); }
uint16_t CMemory::get_word(uint16_t address) {
  return htons(*((u_int16_t *)(m_mem + address)));
}

void CMemory::put_word(uint16_t address, uint16_t w) {
  *((u_int16_t *)(m_mem + address)) = htons(w);
}

uint8_t CMemory::get_byte(uint16_t address) { return *(m_mem + address); }
void CMemory::put_byte(uint16_t address, uint8_t b) { *(m_mem + address) = b; }
std::vector<uint8_t> CMemory::get_bytes(uint16_t address, uint16_t size) {
  return std::vector<uint8_t>((m_mem + address), (m_mem + address + size));
}
void CMemory::put_bytes(uint16_t address, std::vector<uint8_t> b) {
  memcpy((m_mem + address), b.data(), b.size());
}
