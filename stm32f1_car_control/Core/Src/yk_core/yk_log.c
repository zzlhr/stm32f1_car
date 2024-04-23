//
// Created by 刘浩然 on 2024/4/1.
//

#include "yk_log.h"
#include "usart.h"
#include <stdio.h>

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart2, (uint8_t *) &ch, 1, 0xFFFF);
    return ch;
}

char *EM_LOGLevelGet(const int level) {
    if (level == LOG_DEBUG) {
        return "DEBUG";
    } else if (level == LOG_INFO) {
        return "INFO";
    } else if (level == LOG_WARN) {
        return "WARN";
    } else if (level == LOG_ERROR) {
        return "ERROR";
    }
    return "UNLNOW";
}


void YK_LOG_DEBUG(const char *fmt, ...) {
#ifdef OPEN_LOG
    int level = LOG_DEBUG;
    va_list arg;
    va_start(arg, fmt);
    char buf[50] = {0};
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    if (level >= LOG_LEVEL)
        printf("[%-5s] %s \r\n", EM_LOGLevelGet(level), buf);
#endif
}

void YK_LOG_INFO(const char *fmt, ...) {
#ifdef OPEN_LOG
    int level = LOG_INFO;
    va_list arg;
    va_start(arg, fmt);
    char buf[50] = {0};
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    if (level >= LOG_LEVEL)
        printf("[%-5s] %s \r\n", EM_LOGLevelGet(level), buf);
#endif
}

void YK_LOG_WARN(const char *fmt, ...) {
#ifdef OPEN_LOG
    int level = LOG_WARN;
    va_list arg;
    va_start(arg, fmt);
    char buf[50] = {0};
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    if (level >= LOG_LEVEL)
        printf("[%-5s] %s \r\n", EM_LOGLevelGet(level), buf);
#endif
}

void YK_LOG_ERROR(const char *fmt, ...) {
#ifdef OPEN_LOG
    int level = LOG_ERROR;
    va_list arg;
    va_start(arg, fmt);
    char buf[50] = {0};
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    if (level >= LOG_LEVEL)
        printf("[%-5s] %s \r\n", EM_LOGLevelGet(level), buf);
#endif
}





