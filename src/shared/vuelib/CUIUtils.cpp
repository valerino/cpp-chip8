/**
 * @file   CUIUtils.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  user interface utilities, part of vuelib
 */
#include <stdio.h>
#include "CUIUtils.h"

std::string CUIUtils::build_msg_string(int msgtype, const char *msg, va_list args) {
  // generate buffer
  std::string b;
  switch (msgtype) {
  case MSG_INFO:
    b += "[I] ";
    break;
  case MSG_WARNING:
    b += "[X] ";
    break;
  case MSG_ERROR:
    b += "[!] ";
    break;
  default:
    b = "";
  }
  char tmp[1024];
  vsnprintf(tmp, sizeof(tmp), msg, args);
  b += tmp;

  return b;
}

std::string CUIUtils::show_message(int msgtype, const char *msg, ...) {
  // generate buffer
  va_list args;
  va_start(args,msg);
  std::string s = build_msg_string(msgtype, msg, args);
  va_end(args);

  // print on stdio
  // TODO: implement others

  fprintf(stdout, "%s", s.data());

  // also return the string
  return s;
}

std::string CUIUtils::show_toast_message(int msgtype, const char *msg, ...) {
  // TODO: implement
  // generate buffer
  va_list args;
  va_start(args,msg);
  std::string s = build_msg_string(msgtype, msg, args);
  va_end(args);

  // print on stdio
  // TODO: implement others

  fprintf(stdout, "%s", s.data());

  // also return the string
  return s;
}
