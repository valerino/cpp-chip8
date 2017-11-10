//
// Created by valerino on 09/11/2017.
//

#include "CDbg.h"
#include <string>

int CDbg::m_level = DBG_VERBOSE;

void CDbg::print(int level, const char* format, va_list args) {
  if (level < m_level) {
    return;
  }
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
  fprintf(stderr, "[.] %s: %s\n", type, buf);
}

void CDbg::error(const char* format, ...) {
  va_list args;
  va_start(args, format );
  CDbg::print(DBG_ERROR, format, args);
  va_end(args);
}

void CDbg::warning(const char* format, ...) {
  va_list args;
  va_start(args, format );
  CDbg::print(DBG_WARNING, format, args);
  va_end(args);
}

void CDbg::verbose(const char* format, ...) {
  va_list args;
  va_start(args, format );
  CDbg::print(DBG_VERBOSE, format, args);
  va_end(args);

}
void CDbg::notify(const  char* format, ...) {
  va_list args;
  va_start(args, format );
  CDbg::print(DBG_NOTIFY, format, args);
  va_end(args);
}
