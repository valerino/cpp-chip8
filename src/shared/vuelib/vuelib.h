/**
 * @file   vuelib.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  include this on top of every .cpp to use vuelib, usually avoiding including anything else
 */
#ifndef PROJECT_VUELIB_H
#define PROJECT_VUELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "common.h"
#include "IEmulator.h"
#include "vmath.h"
#include "CDbg.h"
#include "CConfiguration.h"
#include "CFile.h"
#include "CMem.h"
#include "CInputUtils.h"
#include "CUIUtils.h"
#include "CDynModule.h"
#include "CInputUtils.h"
#include "CGfxUtils.h"
#include "CTime.h"
#endif // PROJECT_VUELIB_H
