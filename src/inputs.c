// inputs.c
#include "inputs.h"
#include <pthread.h>
#include <stdlib.h>

#define MAX_INPUTS 1024

static NewObject queue[MAX_INPUTS];
static int queue_head = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Inicializa a fila de inputs
void initialize_inputs() {
    pthread_mutex_lock(&mutex);
    queue_head = 0;
    pthread_mutex_unlock(&mutex);
}

// Adiciona novo objeto à fila
void add_new_object(float x, float y, float vx, float vy, int type) {
    pthread_mutex_lock(&mutex);
    if (queue_head < MAX_INPUTS) {
        queue[queue_head++] = (NewObject){ x, y, vx, vy, type };
    }
    pthread_mutex_unlock(&mutex);
}

// Obtém até max_objs novos objetos e limpa a fila
int get_new_objects(NewObject* buffer, int max_objs) {
    pthread_mutex_lock(&mutex);
    int n = (queue_head < max_objs) ? queue_head : max_objs;
    for (int i = 0; i < n; i++) {
        buffer[i] = queue[i];
    }
    // Reset após leitura completa
    queue_head = 0;
    pthread_mutex_unlock(&mutex);
    return n;
}