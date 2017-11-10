cmake_minimum_required(VERSION 3.8)
project(vuelib)

set(SOURCE_FILES vuelib.h common.h IEmulator.h CConfiguration.cpp CConfiguration.h CDbg.cpp CDbg.h)

add_library(vuelib STATIC ${SOURCE_FILES})
