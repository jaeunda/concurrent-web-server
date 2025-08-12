#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_NONE  // disable all logging
} LogLevel;

void log_init(const char *file_path); // if file_path == NULL then console
void log_set_console(int enabled);

// void log_set_level(LogLevel level);

void log_msg(const char *format, ...); // stdout
void log_error(const char *format, ...); // stderr
void log_fatal(const char *format, ...); // stderr -> exit

void log_close(void);


#endif