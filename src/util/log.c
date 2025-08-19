#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>
#include <sys/syscall.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>

#include <fcntl.h>
#include <sys/file.h>
// mutex provides exclusive access to the file, preventing race conditions
#include <pthread.h> 

void lock_file(short lock_type);
static void hup_handler(int sig);

static void get_timestamp(char *, size_t);

static int g_fd = -1;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static LogOpt g_opt;

static volatile sig_atomic_t hup_flag;

int log_init(const LogOpt *opts){ 
    memcpy(&g_opt, opts, sizeof(LogOpt));
    if (pthread_mutex_init(&g_mutex, NULL) != 0){
        return -1;
    }

    pthread_mutex_lock(&g_mutex);
    switch (g_opt.sink) {
        case LOG_SINK_STDOUT:
            g_fd = STDOUT_FILENO;
            break;
        case LOG_SINK_SYSLOG:
            (void)g_fd;
            openlog(opts->syslog_tag, LOG_PID | LOG_CONS, LOG_USER);
            break;
        case LOG_SINK_FILE:
            if ((g_fd = open(opts->file_path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644)) < 0){
                perror("log_init");
                pthread_mutex_unlock(&g_mutex);
                return -1;
            }
            break;
        default:
            pthread_mutex_unlock(&g_mutex);
            return -1;   
    }
    pthread_mutex_unlock(&g_mutex);    

    struct sigaction hup_sa;
    hup_sa.sa_handler = hup_handler;
    sigemptyset(&hup_sa.sa_mask);
    hup_sa.sa_flags = 0;
    sigaction(SIGHUP, &hup_sa, NULL);

    return 0;
}
void log_write(LogLevel level, const char *format, ...){
    if (g_fd <= 0){
        fprintf(stderr, "Error: file descriptor error\n");
        exit(EXIT_FAILURE);
    }
    lock_file(F_WRLCK);
    // write msg
    char log_buffer[BUFFER_SIZE];
    va_list ap;
    char *s;

    va_start(ap, format);
    while (*format){
        
    }

    lock_file(F_UNLCK);
}
static void hup_handler(int sig){
    (void)sig;
    hup_flag = 1;
}
void lock_file(short lock_type){
    struct flock fl = {0};
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = (off_t)0;
    fl.l_len = (off_t)0;
    if (fcntl(g_fd, F_SETLKW, &fl) < 0){
        perror("lock_file");
        exit(EXIT_FAILURE);
    }
}
static void get_timestamp(char *buffer, size_t size){
    char timestamp[128];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (t == NULL){
        perror("localtime");
        exit(EXIT_FAILURE);
    }
    if (strftime(timestamp, sizeof(timestamp), "%c", t) == (size_t)0){
        fprintf(stderr, "strftime returned 0\n");
        exit(EXIT_FAILURE);
    }
    if (snprintf(buffer, size, "[%s]", timestamp) > (int)size){
        fprintf(stderr, "timestamp too long\n");
        exit(EXIT_FAILURE);
    }
    return;
}