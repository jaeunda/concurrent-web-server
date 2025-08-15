#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(void){
#ifdef _MODE_MAIN
    printf("main\n");
#elif _MODE_BLOCKING
    printf("blocking\n");
#elif _MODE_MULTITHREAD
    printf("multi thread\n");
#elif _MODE_THREADPOOL
    printf("thread pool\n");
#elif _MODE_EPOLL
    printf("epoll\n");
#endif

    exit(EXIT_SUCCESS);
}