#include "manager.h"
#include "inputs.h"
#include "worker.h"
#include "fhysics.h"
#include "thread_pool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

ObjectList* quadrant_lists = NULL;
double      total_update_time = 0;
int         total_objects = 0;
Object      objects[MAX_OBJECTS];
ThreadData  thread_data[NUM_QUADRANTS];

// Sincronização para saber quando todas tarefas terminaram
static pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  done_cond = PTHREAD_COND_INITIALIZER;
static int             done_count = 0;

// Função auxiliar para a thread pool
void task_wrapper(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    process_quadrant(data);

    pthread_mutex_lock(&done_mutex);
    done_count++;
    pthread_cond_signal(&done_cond);
    pthread_mutex_unlock(&done_mutex);
}

// Inicializa as listas de quadrantes
static void init_quadrants() {
    quadrant_lists = malloc(sizeof(ObjectList) * NUM_QUADRANTS);
    for (int i = 0; i < NUM_QUADRANTS; i++) {
        quadrant_lists[i].count = 0;
        quadrant_lists[i].capacity = NUM_QUADRANTS;
        quadrant_lists[i].items = malloc(sizeof(Object*) * quadrant_lists[i].capacity);
    }
}

// Libera memória das listas de quadrantes
static void free_quadrants() {
    for (int i = 0; i < NUM_QUADRANTS; i++) {
        free(quadrant_lists[i].items);
    }
    free(quadrant_lists);
}

// Limpa os quadrantes (zera contadores)
static void clear_quadrants() {
    for (int i = 0; i < NUM_QUADRANTS; i++) {
        quadrant_lists[i].count = 0;
    }
}

// Adiciona objeto ao quadrante
static void add_to_quadrant(int q, Object* obj) {
    ObjectList* L = &quadrant_lists[q];
    if (L->count == L->capacity) {
        L->capacity *= 2;
        L->items = realloc(L->items, sizeof(Object*) * L->capacity);
    }
    L->items[L->count++] = obj;
}

// Inicializa o sistema
void initialize_manager() {
    total_objects = 0;
    init_quadrants();
    thread_pool_init(NUM_QUADRANTS);
}

// Atualiza a simulação
void update_simulation() {
    // Começa a medir o tempo
    double frame_start = (double)clock() / CLOCKS_PER_SEC;

    // Processa novos inputs
    NewObject new_objects[128];
    int new_count = get_new_objects(new_objects, 128);
    for (int i = 0; i < new_count && total_objects < MAX_OBJECTS; i++) {
        objects[total_objects++] = (Object){
            new_objects[i].x, new_objects[i].y,
            new_objects[i].vx, new_objects[i].vy,
            new_objects[i].type
        };
    }

    // Redistribui objetos nos quadrantes
    clear_quadrants();
    int grid = (int)sqrt(NUM_QUADRANTS);
    float cell_w = LIMIT_X / (float)grid;
    float cell_h = LIMIT_Y / (float)grid;
    for (int i = 0; i < total_objects; i++) {
        Object* o = &objects[i];
        int gx = fmin(grid - 1, o->x / cell_w);
        int gy = fmin(grid - 1, o->y / cell_h);
        int q  = gy * grid + gx;
        add_to_quadrant(q, o);
    }

    // Prepara e adiciona as tarefas à thread pool
    done_count = 0;
    for (int i = 0; i < NUM_QUADRANTS; i++) {
        thread_data[i].id = i;
        thread_data[i].list = &quadrant_lists[i];
        thread_data[i].processing_time = 0.0;
        thread_pool_add_task(task_wrapper, &thread_data[i]);
    }

    // Espera todas tarefas terminarem
    pthread_mutex_lock(&done_mutex);
    while (done_count < NUM_QUADRANTS) {
        pthread_cond_wait(&done_cond, &done_mutex);
    }
    pthread_mutex_unlock(&done_mutex);

    // Calcula tempo total de atualização
    double frame_end = (double)clock() / CLOCKS_PER_SEC;
    total_update_time = frame_end - frame_start;
}

// Finaliza o sistema
void finalize_manager() {
    thread_pool_destroy();
    free_quadrants();
}
