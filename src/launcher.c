#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/wait.h>
#include "launcher.h"
#include "config.h"
#include "server-modes.h"
int main(int argc, char **argv){
    /* Usage: CONNECT INFORMATION
        1. Server Mode
        2. Port Number (default: 8080)
        3. Bind IP Address (default: 0.0.0.0 or 127.0.0.1)
        4. Document Root (default: ./public)
        5. (ThreadPool) Thread/Worker Count (default: 4)
    */
    static int option;
    static launcherConfig config;
#ifdef _MODE_BLOCK
    option |= OPT_BLOCK;
#elif _MODE_MULTI
    option |= OPT_MULTI;
#elif _MODE_POOL
    option |= OPT_POOL;
#elif _MODE_EPOLL
    option |= OPT_EPOLL;
#endif
/* SERVER MODEL */
    if (!option){
        printf("[Server Mode]\n");
        printf("1. Blocking I/O\n2. Multi-Threaded\n3. Thread Pool\n4. Epoll\n\n");
        int loop = 1;
        while (loop++){
            if (loop > 10) {
                fprintf(stderr, "Error: Too many try\n");
                exit(EXIT_FAILURE);
            }
            printf("Enter the mode number (1-4): ");

            // input number
            int char_cnt = 0;
            char c_tmp, c;
            // get input
            while ((c_tmp = getchar()) != '\n'){
                c = c_tmp;
                char_cnt++;
                if (char_cnt > INPUT_MAX) {
                    fprintf(stderr, "Error: Input too long\n");
                    exit(EXIT_FAILURE);
                }
            }
            // invalid input
            if (char_cnt != 1) continue;
            if (c < '1' || c > '4') continue;
            // option
            switch (c){
                case '1':
                    option |= OPT_BLOCK;
                    break;
                case '2':
                    option |= OPT_MULTI;
                    break;
                case '3':
                    option |= OPT_POOL;
                    break;
                case '4':
                    option |= OPT_EPOLL;
                    break;
            }
            if (!option) continue;
            break;
        }
    }
    
    if (option & OPT_BLOCK){
        strcpy(config.server_model, "server-blocking");
    } else if (option & OPT_MULTI){
        strcpy(config.server_model, "server-multi-thread");
    } else if (option & OPT_POOL){
        strcpy(config.server_model, "server-thread-pool");
    } else if (option & OPT_EPOLL){
        strcpy(config.server_model, "server-epoll");
    } else {
        fprintf(stderr, "Error: cannot run server\n");
        exit(EXIT_FAILURE);
    }

    if (!strcmp(config.server_model, "server-path")){
        fprintf(stderr, "Error: cannot run server\n");
        exit(EXIT_FAILURE);
    }
/* PORT NUMBER */
    int port_number = 8080;
    snprintf(config.port_number, sizeof(config.port_number), "%d", port_number);
/* BIND IP ADDRESS */
    strcpy(config.ip_address, "127.0.0.1");
/* THE PATH FOR STATIC FILES */
    strcpy(config.file_path, "../public");
/* THE NUMBER OF THREADS */
    int thread_number = (option & OPT_POOL) ? 4 : 1;
    snprintf(config.thread_number, sizeof(config.thread_number), "%d", thread_number);

/* Set Current Directory */
    char cur_path[PATH_MAX];
    if (realpath(argv[0], cur_path) == NULL){
        fprintf(stderr, "Error: realpath error\n");
        exit(EXIT_FAILURE);
    }
    
    char *dir_path = dirname(cur_path);
    chdir(dir_path);
    
/* Run server */
    run_server(config);
    printf("[%d] EXIT\n", getpid());

    exit(EXIT_SUCCESS);
}
void run_server(launcherConfig config){
    // argv: [0] pathname, [1] port-number, [2] bind-address, [3] static-file-path, [4] # of threads, [5] NULL
    char *argv[] = {
        config.server_model, 
        config.port_number,
        config.ip_address,
        config.file_path,
        config.thread_number,
        NULL
    };

    /* FORK */
    pid_t pid;
    if ((pid = fork()) < 0){
        fprintf(stderr, "Error: fork error\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) { // server
        if (execv(argv[0], argv) < 0){
            fprintf(stderr, "Error: exec error\n");
            exit(EXIT_FAILURE);
        }
    } else { // parent
        printf("[%d] Starting server with PID %d ...\n", getpid(), pid);
        wait(NULL);
    }

    printf("[%d] Closing server ...\n", getpid());
    return;
}