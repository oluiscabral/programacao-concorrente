#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mtx;

int contador_global = 0;

void *incrementor(void *arg) {
    int n_loops = *(int *)arg;
    for (int i = 0; i < n_loops; i++) {
        pthread_mutex_lock(&mtx);
        contador_global += 1;
        pthread_mutex_unlock(&mtx);
    }
    pthread_exit(NULL); // ou return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);
    pthread_t threads[n_threads];

    pthread_mutex_init(&mtx, NULL);

    for (int i = 0; i < n_threads; i++)
        pthread_create(&threads[i], NULL, incrementor, (void*)&n_loops);

    for (int i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mtx);

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads * n_loops);
    return 0;
}
