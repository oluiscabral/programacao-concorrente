#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>


double* load_vector(const char* filename, int* out_size);

void avaliar(double* a, double* b, int size, double prod_escalar);

typedef struct {
    double result;
    double*first;
    double*second;
    int start;
    int end;
} ScalarInfo;

void* thread(void* arg) {
    ScalarInfo *info = (ScalarInfo *)arg;
    for (int i = info->start; i < info->end; i++) {
        info->result += info->first[i] * info->second[i];
    }
    return 0;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n", 
               argv[0]);
        return 1;
    }
  
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    int max_threads = n_threads;
    if (n_threads > a_size) {
        max_threads = a_size;
    }

    pthread_t threads[max_threads];
    ScalarInfo infos[max_threads];

    int part = a_size / max_threads;
    int start = 0;
    int end = part + a_size % max_threads;
    for (int i = 0; i < max_threads;i++) {
        infos[i] = (ScalarInfo){0, a, b, start, end};
        start = end;
        end += part;
    }

    for (int i = 0; i < max_threads; i++)
        pthread_create(&threads[i], NULL, thread, &infos[i]);

    double result = 0;
    for (int i = 0; i < max_threads; i++) {
        pthread_join(threads[i], NULL);
        result += infos[i].result;
    }

    avaliar(a, b, a_size, result);

    free(a);
    free(b);

    return 0;
}
