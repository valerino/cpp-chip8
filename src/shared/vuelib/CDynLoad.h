//
// Created by valerino on 19/11/2017.
//

#ifndef PROJECT_CDYNLOAD_H
#define PROJECT_CDYNLOAD_H

#ifdef WIN32
#include <windows.h>
#endif

#include <dlfcn.h>
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
   * get exported symbol
   * @param function_name symbol name
   * @return pointer to symbol
   */
  void* getExportedFunction(const char* function_name);

  /**
   * get the IEmulator* interface. must be freed with delete
   * @return
   */
  IEmulator* getIEmulator();

  /**
   * get module path
   * @return
   */
  std::string path() { return std::string(m_path); }

  virtual ~CDynModule();
private:
  void* m_module;
  char m_path[260];
};

#endif //PROJECT_CDYNLOAD_H
