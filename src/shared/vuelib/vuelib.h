//
// Created by valerino on 10/11/2017.
//

#ifndef PROJECT_VUELIB_H
#define PROJECT_VUELIB_H

#ifdef TARGET_OS_WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "common.h"
#include "IEmulator.h"

#include "CDbg.h"
#include "CConfiguration.h"
#include "CFile.h"
#include "CMem.h"
#include "CGfx.h"

#endif // PROJECT_VUELIB_H
