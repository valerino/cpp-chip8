/**
 * @file   CTime.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  time utilities, part of vuelib
 */
#include "CTime.h"
#include <chrono>

uint64_t CTime::now_nanoseconds() {
  // get current time with high resolution
  auto t =std::chrono::high_resolution_clock::now();

  // turn to nanoseconds since epoch
  auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(t);
  return now_ns.time_since_epoch().count();
}
