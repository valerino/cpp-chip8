/**
 * @file   CGfxUtils.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  graphics related utilities, part of vuelib
 */
#ifndef PROJECT_CGFXUTILS_H
#define PROJECT_CGFXUTILS_H

#include <SDL2/SDL.h>
class CGfxUtils {

};

/** @brief convert screen coordinates to flat buffer offset (1 byte per pixel) */
#define SCREEN_TO_FLAT_OFFSET(__x__, __y__, __w__) ((__y__ * __w__) + __x__)

#endif //PROJECT_CGFXUTILS_H
