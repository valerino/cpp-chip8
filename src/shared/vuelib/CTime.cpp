//
// Created by valerino on 12/12/2017.
//

#include "CTime.h"
#include <chrono>
#include <stdint.h>

uint64_t CTime::now_nanoseconds() {
  auto t =std::chrono::high_resolution_clock::now();
  auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(t);
  return now_ns.time_since_epoch().count();
}
