/**
 * @file   CFile.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  file I/O primitives, part of vuelib
 */
#ifndef PROJECT_CFILE_H
#define PROJECT_CFILE_H

#include <stdint.h>
#include <errno.h>
#include <tinydir.h>

class CFile {
public:
  /**
   * create file from buffer
   * @param path path to the file to be created
   * @param buffer buffer to write
   * @param size size of buffer
   * @return 0 on success
   */
  static int from_buffer(const char *path, const void *buffer, int size);

  /**
   * read whole file to buffer
   * @param path path to the file to be read
   * @param size if not null, on successful return is the file size = allocated
   * buffer size
   * @return pointer to allocated buffer, to be freed with CMem::free()
   */
  static const void *to_buffer(const char *path, uint32_t *size = 0);

  /**
   * wraps tinydir_opendir(), returns the tinydir object (refer to tinydir/tinydir.h)
   * call close_directory() when done
   * @param path path to the directory to open
   * @return tinydir_dir*
   */
  static tinydir_dir* open_directory(const char* path);

  /**
   * close directory opened with open_directory()
   * @param dir the tinydir_dir object
   */
  static void close_directory(tinydir_dir* dir);

};

#endif // PROJECT_CFILE_H
