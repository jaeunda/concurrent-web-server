#include "config.h"

#ifndef _LAUNCHER_H
#define _LAUNCHER_H

typedef struct _launcher_config{
    char server_model[BUFFER_SIZE];
    char port_number[BUFFER_SIZE];
    char ip_address[BUFFER_SIZE];
    char file_path[PATH_MAX];
    char thread_number[BUFFER_SIZE];
} launcherConfig; // -> argv

void run_server(launcherConfig config);

#endif