/**
 * @file   vmath.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  math utilities, part of vuelib
 */
#ifndef PROJECT_VMATH_H
#define PROJECT_VMATH_H

/** @brief htons() implementation, swaps byte order in a 16bit value */
#define SWAP_UINT16(__x__) (__x__ << 8) | (__x__ >> 8 )

/** @brief htonl() implementation, swaps byte order in a 32bit value */
#define SWAP_UINT32(__x__) ((__x__>>24)&0xff) | ((__x__<<8)&0xff0000) | ((__x__>>8)&0xff00) | ((__x__<<24)&0xff000000)

#endif //PROJECT_VMATH_H
