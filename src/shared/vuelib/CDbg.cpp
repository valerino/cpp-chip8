/**
 * @file   CDbg.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements debugprint and debug helpers, part of vuelib
 */

#include "CDbg.h"
#include <string>

int CDbg::g_level = DBG_NONE;

void CDbg::print(int level, const char *format, va_list args) {
  char type[32];
  char buf[1024] = {0};
  if (level == DBG_VERBOSE) {
    strcpy(type, "VERBOSE");
  } else if (level == DBG_WARNING) {
    strcpy(type, "WARNING");
  } else if (level == DBG_NOTIFY) {
    strcpy(type, "NOTIFY");
  } else if (level == DBG_ERROR) {
    strcpy(type, "ERROR");
  }
  vsnprintf(buf, sizeof(buf), format, args);

  // print
  fprintf(stderr, "[D] %s: %s\n", type, buf);
}

void CDbg::error(const char *format, ...) {
  if (g_level < DBG_ERROR) {
    return;
  }

  va_list args;
  va_start(args, format);
  CDbg::print(DBG_ERROR, format, args);
  va_end(args);
}

void CDbg::warning(const char *format, ...) {
  if (g_level < DBG_WARNING) {
    return;
  }
  va_list args;
  va_start(args, format);
  CDbg::print(DBG_WARNING, format, args);
  va_end(args);
}

void CDbg::verbose(const char *format, ...) {
  if (g_level < DBG_VERBOSE) {
    return;
  }
  va_list args;
  va_start(args, format);
  CDbg::print(DBG_VERBOSE, format, args);
  va_end(args);
}

void CDbg::notify(const char *format, ...) {
  if (CDbg::g_level < DBG_NOTIFY) {
    return;
  }

  va_list args;
  va_start(args, format);
  CDbg::print(DBG_NOTIFY, format, args);
  va_end(args);
}
void CDbg::set_debug_level(int level) {
  g_level = level;
}
