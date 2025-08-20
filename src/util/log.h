#ifndef LOG_H
#define LOG_H

#include "config.h"

typedef enum {
    LOG_LEVEL_EMERG = 0,
    LOG_LEVEL_ERR = 3,
    LOG_LEVEL_WARN = 4,
    LOG_LEVEL_INFO = 6,
    LOG_LEVEL_DEBUG = 7
} LogLevel;

typedef enum {
    LOG_SINK_FILE = 4,
    LOG_SINK_STDOUT = 2,
    LOG_SINK_SYSLOG = 5
} LogSink;

typedef struct {
    LogSink sink;
    char file_path[PATH_MAX];
    char syslog_tag[BUFFER_SIZE];
    short use_log_thread;
    size_t queue_size;
    short use_flock;
} LogOpt;

// if file_path == NULL then console
int log_init(const LogOpt *opts); // add SIGHUP handler 
void log_write(LogLevel level, const char *format, ...);
extern void log_shutdown(void);
// void log_flush(void);


#endif