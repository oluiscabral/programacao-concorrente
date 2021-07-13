#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int gValue = 0;
pthread_mutex_t gMtx;

void imprimir_resultados(int n, int** results);

void compute(int arg) {
    if (arg < 2) {
        pthread_mutex_lock(&gMtx);
        gValue += arg;
        pthread_mutex_unlock(&gMtx);
    } else {
        compute(arg - 1);
        compute(arg - 2);
    }
}

void* compute_thread(void* arg) {
    int* ret = malloc(sizeof(int));
    pthread_mutex_lock(&gMtx);
    gValue = 0;
    compute(*((int*)arg));
    *ret = gValue;
    pthread_mutex_unlock(&gMtx);
    return ret;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s n_threads x1 x2 ... xn\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    if (!n_threads || argc < 2+n_threads) {
        printf("Uso: %s n_threads x1 x2 ... xn\n", argv[0]);
        return 1;
    }

    pthread_mutexattr_t attrs;
    pthread_mutexattr_init(&attrs);
    pthread_mutexattr_settype(&attrs, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&gMtx, &attrs);

    int args[n_threads];
    int* results[n_threads];
    pthread_t threads[n_threads];
    for (int i = 0; i < n_threads; ++i)  {
        args[i] = atoi(argv[2+i]);
        pthread_create(&threads[i], NULL, compute_thread, &args[i]);
    }
    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], (void**)&results[i]);

    pthread_mutex_destroy(&gMtx);
    pthread_mutexattr_destroy(&attrs);

    imprimir_resultados(n_threads, results);

    for (int i = 0; i < n_threads; ++i) 
        free(results[i]);
    
    return 0;
}
