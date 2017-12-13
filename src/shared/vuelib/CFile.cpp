/**
 * @file   CFile.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  file I/O primitives, part of vuelib
 */

#include "CFile.h"
#include "CMem.h"
#include <SDL2/SDL.h>

int CFile::from_buffer(const char *path, const void *buffer, int size) {
  if (!path || !buffer || !size) {
    return EINVAL;
  }

  // open file for writing
  SDL_RWops *rw = SDL_RWFromFile(path, "w+b");
  if (rw == NULL) {
    return EIO;
  }

  // write
  size_t written = SDL_RWwrite(rw, buffer, size, 1);

  // close file
  SDL_RWclose(rw);
  if (written != 1) {
    return EIO;
  }

  return 0;
}

const void *CFile::to_buffer(const char *path, uint32_t *size) {
  if (!path || !size) {
    errno = EINVAL;
    return NULL;
  }
  *size = 0;

  // open file
  SDL_RWops *rw = SDL_RWFromFile(path, "rb");
  if (rw == NULL) {
    errno = EIO;
    return NULL;
  }

  // get file size
  uint64_t file_size = SDL_RWsize(rw);
  if (file_size <=0) {
    SDL_RWclose(rw);
    errno = EIO;
    return NULL;
  }

  uint8_t* buffer = (uint8_t*)CMem::alloc(file_size + 1);
  if (!buffer) {
    // out of memory
    SDL_RWclose(rw);
    errno = ENOMEM;
    return NULL;
  }

  // read whole file to memory
  size_t read = SDL_RWread(rw,buffer, file_size, 1);
  if (read != 1) {
    // file io error
    CMem::free(buffer);
    SDL_RWclose(rw);
    errno = EIO;
    return NULL;
  }

  // done
  *size = file_size;
  return buffer;
}

tinydir_dir *CFile::open_directory(const char *path) {
  if (!path) {
    errno=EINVAL;
    return NULL;
  }
  tinydir_dir* d = new tinydir_dir();
  int res = tinydir_open(d,path);
  if (res != 0) {
    delete  d;
    errno=ENOENT;
    return NULL;
  }
  return d;
}

void CFile::close_directory(tinydir_dir *dir) {
  if (dir) {
    tinydir_close(dir);
    delete dir;
  }
}

