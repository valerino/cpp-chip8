//
// Created by valerino on 12/12/2017.
//

#ifndef PROJECT_CTIME_H
#define PROJECT_CTIME_H

#include <stdint.h>
class CTime {
  /**
   * get current time in nanoseconds, since unix epoch
   * @return
   */
public:
  static uint64_t now_nanoseconds();
};

#endif //PROJECT_CTIME_H
