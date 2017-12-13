/**
 * @file   CDynModule.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  dynamic library loader used to load cores, part of vuelib
 */
#ifndef PROJECT_CDYNLOAD_H
#define PROJECT_CDYNLOAD_H

#include "IEmulator.h"

/**
 * manages dynamic loaded libraries
 */
class CDynModule {
public:
  /**
   * load dynamic module
   * @param path path to the dynamic library MINUS the extension (will be added by the loader)
   * (i.e. to load foo: CDynModule("path/to/libfoo") will load libfoo.dll on windows, libfoo.so on linux, libfoo.dylib on apple)
   * @throws std::system_error when load fails
   */
  CDynModule(const char* path);

  /**
   * get the IEmulator* interface, must be freed with delete
   * @return
   */
  IEmulator* getIEmulator();

  /**
   * get loaded module path
   * @return
   */
  std::string path() { return std::string(m_path); }

  /**
   * destructor
   */
  virtual ~CDynModule();

private:
  void* m_module;
  char m_path[260];
};

#endif //PROJECT_CDYNLOAD_H
