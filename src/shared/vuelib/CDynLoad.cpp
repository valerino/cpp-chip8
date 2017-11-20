//
// Created by valerino on 19/11/2017.
//
#include "CDynLoad.h"
#include <stdio.h>
#include <string>
#include <fstream>

CDynModule::CDynModule(const char *path) {
  strcpy(m_path,path);
#ifdef _WIN32
  strcat(p,".dll");
  m_module = LoadLibraryA(p);
  }
#elif __APPLE__
#include "TargetConditionals.h"
  strcat(m_path,".dylib");
  m_module = dlopen(m_path,RTLD_LAZY);
#elif __linux__
  strcat(m_path,".so");
  m_module = dlopen(p,RTLD_LAZY);
#endif
  if (!m_module) {
#ifdef _WIN32
    throw std::system_error(GetLastError(), std::generic_category(), std::string());
#else
    throw std::system_error(errno, std::generic_category(), std::string(dlerror()));
#endif
  }
}

void *CDynModule::getExportedFunction(const char *function_name) {
  void* func = NULL;
#ifdef WIN32
  func = (void*)GetProcAddress(m_module, function_name);
#else
  func = (void*)dlsym(m_module, function_name);
#endif
  return func;
}


CDynModule::~CDynModule() {
  if (m_module) {
#ifdef WIN32
    FreeLibrary((HMODULE)m_module);
#else
    dlclose(m_module);
#endif
  }
}

IEmulator *CDynModule::getIEmulator() {
  GET_EMULATOR_INTERFACE p =
      (GET_EMULATOR_INTERFACE)getExportedFunction("get_emulator_interface");
  if (!p) {
    return NULL;
  }

  IEmulator *e = (IEmulator *)p();
  return e;
}
