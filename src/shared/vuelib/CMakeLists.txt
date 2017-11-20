cmake_minimum_required(VERSION 3.8)
project(vuelib)

set(SOURCE_FILES vuelib.h common.h IEmulator.h CConfiguration.cpp CConfiguration.h CDbg.cpp CDbg.h CFile.cpp CFile.h CMem.h CSDLUtils.cpp CSDLUtils.h CDynLoad.cpp CDynLoad.h CUIUtils.cpp CUIUtils.h)

add_library(vuelib STATIC ${SOURCE_FILES})
target_include_directories(vuelib PUBLIC json/src tinydir)
