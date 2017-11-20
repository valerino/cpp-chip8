//
// Created by valerino on 09/11/2017.
//

#ifndef PROJECT_CDBG_H
#define PROJECT_CDBG_H

#include <stdarg.h>

/**
 * debug levels
 */
#define DBG_VERBOSE 0
#define DBG_ERROR 1
#define DBG_WARNING 2
#define DBG_NOTIFY 3
#define DBG_NONE 4

/**
 * debug print primitives
 */
class CDbg {

private:
  static void print(int level, const char *format, va_list args);
  static int m_level;

public:
  /**
   * sets the debug level, higher masks lower
   * @param type DBG_VERBOSE, DBG_NOTIFY, DBG_WARNING, DBG_ERROR, DBG_NONE
   */
  static void set_debug_level(int level) { m_level = level; }

  /**
   * prints an error message
   * @param format format string
   * @param ... variadic
   */
  static void error(const char *format, ...);

  /**
   * prints a warning message
   * @param format format string
   * @param ... variadic
   */
  static void warning(const char *format, ...);

  /**
   * prints a verbose message
   * @param format format string
   * @param ... variadic
   */
  static void verbose(const char *format, ...);

  /**
   * prints a notify message
   * @param format format string
   * @param ... variadic
   */
  static void notify(const char *format, ...);
};

#endif // PROJECT_CDBG_H
