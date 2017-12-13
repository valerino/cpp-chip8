/**
 * @file   CUIUtils.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  user interface utilities, part of vuelib
 */
#ifndef PROJECT_CUTILS_H
#define PROJECT_CUTILS_H

/** @brief display an 'info' message */
#define MSG_INFO 0
/** @brief display an 'error' message */
#define MSG_ERROR 1
/** @brief display a 'warning' message */
#define MSG_WARNING 2

#include <string>
#include <stdarg.h>

/**
 * implement generic utilities
 */
class CUIUtils {
public:
  /**
   * show message depending on the platform and ui (may be messagebox or console)
   * @param msgtype MSG_INFO, MSG_ERROR, MSG_WARNING
   * @param msg the message
   * @param ... variadic
   * @return std::string the message
   */
  static std::string show_message(int msgtype, const char *msg, ...);

  /**
   * show toast (flash) message depending on the platform and ui
   * @param msgtype MSG_INFO, MSG_ERROR, MSG_WARNING
   * @param msg the message
   * @param ... variadic
   * @return std::string the message
   */
  static std::string show_toast_message(int msgtype, const char *msg, ...);

private:
  static std::string build_msg_string(int msgtype, const char* msg, va_list args);
};

#endif // PROJECT_CUTILS_H
