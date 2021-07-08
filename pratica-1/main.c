#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

int *init_array(int size) {
    int *array = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
    return array;
}

/// Reserva uma região de memória capaz de guardar capacity ints e
/// inicializa os atributos de b
void init_buffer(buffer_t *b, int capacity) {
    b->data = init_array(capacity);
    b->put_idx = 0;
    b->take_idx = 0;
    b->capacity = capacity;
    b->size = 0;
}

/// Libera a memória e quaisquer recursos de propriedade de b. Desfaz o
/// init_buffer()
void destroy_buffer(buffer_t *b) {
    free(b->data);
}

int next_idx(int current, int array_len) {
    if (current == array_len-1) {
        return 0;
    }
    return current + 1;
}

int take_value(buffer_t *b) {
    int take_idx = b->take_idx;
    int val = b->data[take_idx];
    b->data[take_idx] = 0;
    return val;
}

/// Retorna o valor do elemento mais antigo do buffer b, ou retorna -1 se o
/// buffer estiver vazio
int take_buffer(buffer_t *b) {
    if (b->size == 0) {
        return -1;
    }
    int val = take_value(b);
    b->take_idx = next_idx(b->take_idx, b->capacity);
    b->size -= 1;
    return val;
}

/// Adiciona um elemento ao buffer e retorna 0, ou retorna -1 sem
/// alterar o buffer se não houver espaço livre
int put_buffer(buffer_t *b, int val) {
    if (b->size == b->capacity) {
        return -1;
    }
    b->data[b->put_idx] = val;
    b->put_idx = next_idx(b->put_idx, b->capacity);
    b->size += 1;
    return 0;
}

/// Lê um comando do terminal e o executa. Retorna 1 se o comando era
/// um comando normal. No caso do comando de terminar o programa,
/// retorna 0
int ler_comando(buffer_t *b);

int main(int argc, char **argv) {

    int capacity = 0;
    printf("Digite o tamanho do buffer:\n>");
    if (scanf("%d", &capacity) <= 0) {
        printf("Esperava um número\n");
        return 1;
    }
    buffer_t b;
    init_buffer(&b, capacity);

    printf("Comandos:\n"
           "r: retirar\n"
           "c: colocar\n"
           "q: sair\n\n");

    int comando_ret;
    do {
        comando_ret = ler_comando(&b);
    } while (comando_ret);

    destroy_buffer(&b);
    return 0;
}
