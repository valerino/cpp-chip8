//
// Created by valerino on 08/11/2017.
//

#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H

#define EXPORT __attribute__((visibility("default")))

#define SAFE_DELETE(__x__) if (__x__) { delete __x__; __x__=NULL; }

#endif //PROJECT_COMMON_H
