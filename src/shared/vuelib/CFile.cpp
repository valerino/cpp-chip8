//
// Created by valerino on 12/11/2017.
//

#include "CFile.h"
#include "CMem.h"
#include <errno.h>
#include <fstream>

int CFile::from_buffer(const char *path, const void *buffer, int size) {
  std::ofstream os(path, std::ios::out);
  if (!os) {
    return ENOENT;
  }
  os.write((char *)buffer, size);
  os.close();
  return 0;
}

const void *CFile::to_buffer(const char *path, uint32_t *size) {
  std::ifstream is(path, std::ifstream::binary);
  if (!is) {
    return NULL;
  }
  if (size) {
    *size = 0;
  }

  // get length of file:
  is.seekg(0, is.end);
  int length = is.tellg();
  if (length == 0) {
    is.close();
    return NULL;
  }

  is.seekg(0, is.beg);
  char *buffer = (char *)CMem::alloc(length);
  if (!buffer) {
    is.close();
    return NULL;
  }

  // read data as a block:
  is.read(buffer, length);
  is.close();
  if (size) {
    *size = length;
  }
  return buffer;
}

tinydir_dir *CFile::open_directory(const char *path) {
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

