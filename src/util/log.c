#include "../include/util/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

static void get_timestamp(char *, size_t);

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