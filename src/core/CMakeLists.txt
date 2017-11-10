cmake_minimum_required(VERSION 3.8)
project(core)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES core.cpp ${VUELIB_SRC_DIR}/vuelib.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR})

add_executable(core ${SOURCE_FILES})

target_link_libraries(core vuelib)