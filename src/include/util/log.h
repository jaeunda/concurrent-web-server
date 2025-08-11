#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

// typedef enum {
//     LOG_LEVEL_DEBUG = 0,
//     LOG_LEVEL_INFO,
//     LOG_LEVEL_WARN,
//     LOG_LEVEL_ERROR,
//     LOG_LEVEL_FATAL,
//     LOG_LEVEL_NONE  // disable all logging
// } LogLevel;

// void log_init(const char *file_path, LogLevel level);
// void log_set_level(LogLevel level);
// LogLevel log_get_level(void);
// void log_msg(LogLevel level, const char *fmt, ...);
// void log_close(void);

static void get_timestamp(char *buffer, size_t size);
// stdout
void log_msg(const char *format, ...); 
// stderr and exit
void log_error(const char *format, ...);

#endif