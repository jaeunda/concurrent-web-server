#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char **argv){
    printf("[%d] Multi-Threaded Server Running ...\n", getpid());
    for (int i=3; i>0; i--){
        printf("[%d] Close in %d seconds ...\n", getpid(), i);
        sleep(1);
    }
    printf("[%d] Exit Multi-Threaded Server.\n", getpid());
    exit(EXIT_SUCCESS);
}