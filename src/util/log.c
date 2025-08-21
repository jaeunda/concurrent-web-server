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

#include "log.h"

void lock_file(short lock_type);
static void hup_handler(int sig);
static void rotate_log_file(void);

int append_pid(char *file_name, size_t size);
char *get_timestamp(void);
const char *get_LogLevel(LogLevel level);

static int g_fd = -1;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static LogOpt g_opt;

static volatile sig_atomic_t hup_flag = 0;
// static atomic_bool g_inited = false;

#ifdef _DEBUG_LOG
int main(void){ 
    LogOpt opt = {0};
    opt.sink = LOG_SINK_STDOUT;
    opt.use_flock = 1;

    #ifdef _SYSLOG
    opt.sink = LOG_SINK_SYSLOG;
    strcpy(opt.syslog_tag, "log_test");
    opt.use_flock = 1;
    #endif

    #ifdef _FILE
    opt.sink = LOG_SINK_FILE;
    strcpy(opt.file_path, "./test_log");
    opt.use_flock = 1;
    #endif

    log_init(&opt);
    for (int i=0; i<10; i++)
        log_write(LOG_LEVEL_INFO, "%d log_test", getpid());
    log_shutdown();
    return 0;
}
#endif

int log_init(const LogOpt *opts){ 
    memcpy(&g_opt, opts, sizeof(LogOpt));
    if (append_pid(g_opt.file_path, sizeof(g_opt.file_path)) < 0){
        perror("append_pid");
        return -1;
    }

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
            if ((g_fd = open(g_opt.file_path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644)) < 0){
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
    if (g_opt.sink & LOG_SINK_FILE){
        if (hup_flag)
            rotate_log_file();
        if (g_fd < 0){
            fprintf(stderr, "Error: log file error\n");
            return;
        }
    }
    // buffering
    char str_buffer[BUFFER_SIZE/2];
    memset((char *)str_buffer, 0, sizeof(str_buffer));

    va_list args;
    va_start(args, format);
    vsprintf(str_buffer, format, args);
    va_end(args);

    char log_buffer[BUFFER_SIZE];
    memset((char *)log_buffer, 0, sizeof(log_buffer));
    snprintf(log_buffer, sizeof(log_buffer), "[%s][%s][%d][%lu] %s\n", get_timestamp(), get_LogLevel(level), (int)getpid(), (unsigned long)pthread_self(), str_buffer);

    pthread_mutex_lock(&g_mutex);
    if (g_opt.sink & LOG_SINK_FILE) lock_file(F_WRLCK);

    if (g_opt.sink & LOG_SINK_SYSLOG){
        syslog(level, "%s", str_buffer);
    } else {
        if (write(g_fd, log_buffer, strlen(log_buffer)) < 0){
            if (g_opt.sink & LOG_SINK_FILE) lock_file(F_UNLCK);
            pthread_mutex_unlock(&g_mutex);
            perror("log_write");
            return;  // exit
        }
    }
    if (g_opt.sink & LOG_SINK_FILE) lock_file(F_UNLCK);
    pthread_mutex_unlock(&g_mutex);
}
void log_shutdown(void){
    if (g_opt.sink & LOG_SINK_SYSLOG){
        closelog();
    } else {
        close(g_fd);
    }
    pthread_mutex_destroy(&g_mutex);
    return;
}
static void rotate_log_file(void){
    if (!(g_opt.sink & LOG_SINK_FILE))
        return;
    if (g_fd < 0)
        return;    

    pthread_mutex_lock(&g_mutex);
    
    close(g_fd);
    if ((g_fd = open(g_opt.file_path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644)) < 0){
        fprintf(stderr, "%s: log file rotation failed\n", g_opt.file_path);
        pthread_mutex_unlock(&g_mutex);
        return; // exit
    }
    hup_flag = 0;
    pthread_mutex_unlock(&g_mutex);
    return; 
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
char *get_timestamp(void){
    static __thread char timestamp[128];
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
    return timestamp;
}
const char *get_LogLevel(LogLevel level){
    switch (level){
        case LOG_LEVEL_EMERG: return "EMERGE";
        case LOG_LEVEL_ERR: return "ERROR";
        case LOG_LEVEL_WARN: return "WARN";
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}
int append_pid(char *file_name, size_t size){
    if (!(g_opt.sink & LOG_SINK_FILE)) return 0;
    char pid_buffer[32];
    snprintf(pid_buffer, sizeof(pid_buffer), ".%d", getpid());
    
    size_t len = strlen(file_name);
    if (len + strlen(pid_buffer) > size) return -1;
    memcpy(file_name + len, pid_buffer, strlen(pid_buffer)+1);
    return 0;
}