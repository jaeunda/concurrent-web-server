#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "log.h"

void clean_resource(void);

void err_warn(const char *format, ...){
    va_list args;
    va_start(args, format);

    char buffer[BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log_write(LOG_LEVEL_WARN, "%s", buffer);
    return;
}
void err_fatal(const char *format, ...){
    va_list args;
    va_start(args, format);

    char buffer[BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log_write(LOG_LEVEL_EMERG, "%s", buffer);

    clean_resource();
    exit(EXIT_FAILURE);
}
void clean_resource(void){
    log_shutdown();
    // else
    return;
}