#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv) {
    
    int pid = 0;

    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid > 0) {
            printf("Processo pai criou %d\n", pid);
        } else if (pid == 0) {
            printf("Processo filho %d criado\n", getpid());
            break;
        }
    }

    if (pid > 0) {
        wait(NULL);
        printf("Processo pai finalizado!\n");
    }

    return 0;
}
