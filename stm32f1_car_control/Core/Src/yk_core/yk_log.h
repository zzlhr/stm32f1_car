//
// Created by 刘浩然 on 2024/4/1.
//

#ifndef YT_2_YK_LOG_H
#define YT_2_YK_LOG_H

#include <stdarg.h>

#define OPEN_LOG 1
#define LOG_LEVEL LOG_DEBUG

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} E_LOGLEVEL;

void YK_LOG_DEBUG(const char *fmt, ...);

void YK_LOG_INFO(const char *fmt, ...);

void YK_LOG_WARN(const char *fmt, ...);

void YK_LOG_ERROR(const char *fmt, ...);

#define EMLOG(level, fmt, ...) EM_LOG(level,__FUNCTION__,__LINE__,fmt, ##__VA_ARGS__)


#endif //YT_2_YK_LOG_H
