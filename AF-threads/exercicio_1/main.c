#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>


int contador_global = 0;

void* thread(void* arg) {
    int n_loop = *(int *)arg;
    for (int i = 0; i < n_loop; i++) {
        contador_global++;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);

    pthread_t threads[n_threads];

    for (int i = 0; i < n_threads; ++i)
        pthread_create(&threads[i], NULL, thread, &n_loops);

    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], NULL);

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    
    return 0;
}
