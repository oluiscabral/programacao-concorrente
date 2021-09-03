#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int produzir(int value);    //< definida em helper.c
void consumir(int produto); //< definida em helper.c
void *produtor_func(void *arg);
void *consumidor_func(void *arg);

int indice_produtor, indice_consumidor, tamanho_buffer;
int* buffer;

sem_t licencas;

//Você deve fazer as alterações necessárias nesta função e na função
//consumidor_func para que usem semáforos para coordenar a produção
//e consumo de elementos do buffer.
void *produtor_func(void *arg) {
    //arg contem o número de itens a serem produzidos
    int max = *((int*)arg);
    for (int i = 0; i <= max; ++i) {
        int produto;
        if (i == max)
            produto = -1;          //envia produto sinlizando FIM
        else 
            produto = produzir(i); //produz um elemento normal
        indice_produtor = (indice_produtor + 1) % tamanho_buffer; //calcula posição próximo elemento
        sem_wait(&licencas);
        buffer[indice_produtor] = produto; //adiciona o elemento produzido à lista
        sem_post(&licencas);
    }
    return NULL;
}

void *consumidor_func(void *arg) {
    while (1) {
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer; //Calcula o próximo item a consumir
        sem_wait(&licencas);
        int produto = buffer[indice_consumidor]; //obtém o item da lista
        sem_post(&licencas);
        //Podemos receber um produto normal ou um produto especial
        if (produto >= 0)
            consumir(produto); //Consome o item obtido.
        else
            break; //produto < 0 é um sinal de que o consumidor deve parar
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s tamanho_buffer itens_produzidos n_produtores n_consumidores \n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    int itens = atoi(argv[2]);
    int n_produtores = atoi(argv[3]);
    int n_consumidores = atoi(argv[4]);
    printf("itens=%d, n_produtores=%d, n_consumidores=%d\n",
	   itens, n_produtores, n_consumidores);

    //Iniciando buffer
    indice_produtor = 0;
    indice_consumidor = 0;
    buffer = malloc(sizeof(int) * tamanho_buffer);

    sem_init(&licencas, 0, 1);

    pthread_t prod_thread[n_produtores];
    pthread_t cons_thread[n_consumidores];
    for (int i = 0; i < n_produtores || i < n_consumidores; i++) {
        if (i < n_produtores)
            pthread_create(&prod_thread[i], NULL, produtor_func, (void *)&itens);
        if (i < n_consumidores)
            pthread_create(&cons_thread[i], NULL, consumidor_func, NULL);
    }

    // Espera threads para produtor_func e consumidor_func
    for (int i = 0; i < n_produtores; i++)
        pthread_join(prod_thread[i], NULL);
    for (int i = 0; i < n_consumidores; i++)
        pthread_join(cons_thread[i], NULL);

    // Destrói semáforos
    sem_destroy(&licencas);

    //Libera memória do buffer
    free(buffer);

    return 0;
}

