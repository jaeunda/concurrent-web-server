#ifndef LOG_H
#define LOG_H

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

typedef enum {
    LOG_SINK_FILE = 0,
    LOG_SINK_STDOUT,
    LOG_SINK_SYSLOG
} LogSink;

typedef struct {
    LogSink sink;
    const char *log_path;
    const char *syslog_tag;
    short use_log_thread;
    size_t queue_size;
    short use_flock;
} LogOpt;

// if file_path == NULL then console
// return fd;
int log_init(const LogOpt *opts); // add SIGHUP handler 

void log_write(LogLevel level, const char *format, ...);

void log_flush(void);
void log_shutdown(void);


#endif