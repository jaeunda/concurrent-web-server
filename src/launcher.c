#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/launcher-mode.h"
int main(int argc, char **argv){
    // usage: connect information
    static int option;
#ifdef _MODE_BLOCK
    option |= OPT_BLOCK;
#elif _MODE_MULTI
    option |= OPT_MULTI;
#elif _MODE_POOL
    option |= OPT_POOL;
#elif _MODE_EPOLL
    option |= OPT_EPOLL;
#endif
    if (!option){
        printf("[Server Mode]\n");
        printf("1. Blocking I/O\n2. Multi-Threaded\n3. Thread Pool\n4. Epoll\n\n");
        int loop = 1;
        while (loop++){
            if (loop > 10) {
                fprintf(stderr, "Error: Too many try\n");
                exit(EXIT_FAILURE);
            }
            printf("Enter the mode number (1-3): ");

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

    switch (option){
        case OPT_BLOCK:
            printf("blocking\n");
            break;
        case OPT_MULTI:
            printf("multi threaded\n");
            break;
        case OPT_POOL:
            printf("thread pool\n");
            break;
        case OPT_EPOLL:
            printf("epoll\n");
            break;
        default:
            fprintf(stderr, "there is no option\n");
            exit(EXIT_FAILURE);
    }

    // argv: path, port-number, bind-address, static-file-path, NULL
    exit(EXIT_SUCCESS);
}