cmake_minimum_required(VERSION 3.8)

# setup
set(CMAKE_CXX_STANDARD 11)

# directories
set(BASE_DIR ${CMAKE_SOURCE_DIR}/bin)
set(SHARED_SRC_DIR ${CMAKE_SOURCE_DIR}/src/shared)
set(VUELIB_SRC_DIR ${SHARED_SRC_DIR}/vuelib)
set(EMULATORS_DIR ${BASE_DIR}/emulators)

# sdl
set (SDL_VERSION "2")
set (SDL_PATH "/usr/local/opt/sdl${SDL_VERSION}")
set (SDL_INCLUDE_PATH "${SDL_PATH}/include")
set (SDL_LIB_PATH "${SDL_PATH}/lib")
set (SDL_LIB SDL${SDL_VERSION})

# includes
include_directories (${SDL_INCLUDE_PATH} ${VUELIB_SRC_DIR})

# libraries
link_directories (${SDL_LIB_PATH} ${VUELIB_SRC_DIR})

# projects
add_subdirectory(src/shared)
add_subdirectory(src/core)
add_subdirectory(src/emulators)
