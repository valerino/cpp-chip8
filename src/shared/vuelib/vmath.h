//
// byte swapping and math utilities
// Created by valerino on 09/12/2017.
//

#ifndef PROJECT_VMATH_H
#define PROJECT_VMATH_H

#define SWAP_UINT16(__x__) (__x__ << 8) | (__x__ >> 8 )
#define SWAP_UINT32(__x__) ((__x__>>24)&0xff) | ((__x__<<8)&0xff0000) | ((__x__>>8)&0xff00) | ((__x__<<24)&0xff000000)

#endif //PROJECT_VMATH_H
