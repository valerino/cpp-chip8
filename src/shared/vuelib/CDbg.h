/**
 * @file   CDbg.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements debugprint and debug helpers, part of vuelib
 */
#ifndef PROJECT_CDBG_H
#define PROJECT_CDBG_H

#include <stdarg.h>

/** @brief for set_debug_level(), no debug messages are displayed */
#define DBG_NONE 0
/** @brief for set_debug_level(), display notify messages only */
#define DBG_NOTIFY 1
/** @brief for set_debug_level(), display warning and notify messages */
#define DBG_WARNING 2
/** @brief for set_debug_level(), display warning, error and notify messages */
#define DBG_ERROR 3
/** @brief for set_debug_level(), display all messages */
#define DBG_VERBOSE 4

/**
 * debug print primitives
 */
class CDbg {

public:
  /**
   * sets the debug level, lower level masks higher
   * @param type DBG_VERBOSE, DBG_NOTIFY, DBG_WARNING, DBG_ERROR, DBG_NONE
   */
  static void set_debug_level(int level);

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

private:
  static void print(int level, const char *format, va_list args);
  static int g_level;
};

#endif // PROJECT_CDBG_H
