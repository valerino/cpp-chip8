/**
 * @file   CDynModule.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  dynamic library loader used to load cores, part of vuelib
 */
#include "CDynModule.h"
#include <fstream>
#include <SDL2/SDL.h>

CDynModule::CDynModule(const char *path) {
  // build dynamic library path according to the running OS
  strcpy(m_path,path);
#ifdef _WIN32
  strcat(p,".dll");
  }
#elif __APPLE__
  strcat(m_path,".dylib");
#elif __linux__
  strcat(m_path,".so");
#endif

  // load through SDL
  m_module = SDL_LoadObject(m_path);
  if (!m_module) {
    // throw exception
    throw std::system_error(ENOENT, std::generic_category(), std::string(SDL_GetError()));
  }
}

CDynModule::~CDynModule() {
  if (m_module) {
    SDL_UnloadObject(m_module);
  }
}

IEmulator *CDynModule::getIEmulator() {
  // resolve the get_emulator_interface function
  GET_EMULATOR_INTERFACE p =
      (GET_EMULATOR_INTERFACE)SDL_LoadFunction(m_module, "get_emulator_interface");
  if (!p) {
    return NULL;
  }

  // get pointer to the IEmulator interface
  IEmulator *e = p();
  return e;
}
