#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"
int main(int argc, char **argv){
// LOG
    LogOpt log_opt = {0};
    log_opt.sink = LOG_SINK_FILE;
    strcpy(log_opt.file_path, "./tests/log");
    log_init(&log_opt);

    if (argc != 5){
        log_write(LOG_LEVEL_ERR, "Error: argument error\n");
        exit(EXIT_FAILURE);
    }

    log_write(LOG_LEVEL_INFO, "Blocking Server Running on %s:%s...", argv[2], argv[1]); 
    for (int i=3; i>0; i--){
        log_write(LOG_LEVEL_INFO, "Close in %d seconds ...", i);
        sleep(1);
    }
    log_write(LOG_LEVEL_INFO, "Exit Blocking Server");

    log_shutdown();
    exit(EXIT_SUCCESS);
}