#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

void child_message() {
    printf("Processo %d, filho de %d\n", getpid(), getppid());
}

void grandchild_process() {
    child_message();
    sleep(5);
    printf("Processo %d finalizado\n", getpid());
}

void child_process() {

    child_message();

    int pid = 0;

    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            grandchild_process();
            break;
        }
    }

    if (pid > 0) {
        wait(NULL);
        printf("Processo %d finalizado\n", getpid());
    }
}

int main(int argc, char** argv) {
    
    int pid = 0;

    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid == 0) {
            child_process();
            break;
        }
    }

    if (pid > 0) {
        wait(NULL);
        printf("Processo principal %d finalizado!\n", getpid());
    }

    return 0;
}
