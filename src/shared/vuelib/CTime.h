/**
 * @file   CTime.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  time utilities, part of vuelib
 */
#ifndef PROJECT_CTIME_H
#define PROJECT_CTIME_H

#include <stdint.h>
class CTime {
  /**
   * get current time in nanoseconds, since unix epoch (1/1/1970)
   * @return
   */
public:
  static uint64_t now_nanoseconds();
};

#endif //PROJECT_CTIME_H
