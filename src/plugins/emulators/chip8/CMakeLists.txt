cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project(chip8)

set(SOURCE_FILES ${VUELIB_SRC_DIR}/vuelib.h CEmuChip8.cpp CEmuChip8.h CDisplay.cpp CDisplay.h CInput.cpp CInput.h CCpu.cpp CCpu.h CMemory.cpp CMemory.h defs.h)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EMULATORS_DIR}/${PROJECT_NAME})

add_library(chip8 SHARED ${SOURCE_FILES})

target_link_libraries(chip8 vuelib SDL2)