#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>


int sed_process() {
    printf("sed PID %d iniciado\n", getpid());
    execlp("sed", "sed", "-i", "s/silver/axamantium/g;s/adamantium/silver/g;s/axamantium/adamantium/g", "text", NULL);
    return 0;
}

int grep_process() {
    printf("grep PID %d iniciado\n", getpid());
    execlp("grep", "grep", "adamantium", "text", NULL);
    return 0;
}

void print_grep_status(int status) {
    if (status == 0) {
        printf("grep retornou com código 0, encontrou adamantium\n");
    } else {
        printf("grep retornou com código %d, não encontrou adamantium\n", status);
    }
}

int main(int argc, char** argv) {
    printf("Processo pai iniciado\n");

    int sed_pid = fork();
    if (sed_pid == 0) {
        return sed_process();
    } else if (sed_pid > 0) {
        wait(NULL);
    }

    int grep_pid = fork();
    if (grep_pid == 0) {
        return grep_process();
    } else if (grep_pid > 0) {
        int status;
        wait(&status);
        print_grep_status(status);
    }

    return 0;
}
