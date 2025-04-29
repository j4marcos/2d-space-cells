#include "thread_pool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Task {
    void (*function)(void*);
    void* arg;
    struct Task* next;
} Task;
// Estrutura da thread pool
typedef struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t* threads;
    Task* tasks_head;
    Task* tasks_tail;
    int thread_count;
    int shutdown;
} ThreadPool;

static ThreadPool pool;

// Função que executa a rotina de trabalho da thread, Cada thread executa essa função em loop
static void* thread_do_work(void* arg) {
    (void)arg;
    while (1) {
        // Bloqueia o Mutex para acessar à fila de tarefas
        pthread_mutex_lock(&pool.lock);
        while (pool.tasks_head == NULL && !pool.shutdown) {
            pthread_cond_wait(&pool.notify, &pool.lock);
        }

        if (pool.shutdown) {
            pthread_mutex_unlock(&pool.lock);
            pthread_exit(NULL);
        }

        Task* task = pool.tasks_head;// Pega a próxima tarefa da fila
        if (task) {
            pool.tasks_head = task->next;
            if (pool.tasks_head == NULL) {
                pool.tasks_tail = NULL;
            }
        }

        //Libera o mutex para outas threads e executa a tarefa, caso haja.
        pthread_mutex_unlock(&pool.lock);
        if (task) {
            task->function(task->arg);
            free(task);
        }
    }
    return NULL;
}

//Inicializa a thread pool com número especificado.
void thread_pool_init(int num_threads) {
    pool.thread_count=  num_threads;
    pool.shutdown=0;
    pool.tasks_head = NULL;
    pool.tasks_tail = NULL;
    pthread_mutex_init(&pool.lock,NULL);
    pthread_cond_init(&pool.notify, NULL);
    pool.threads = malloc(sizeof(pthread_t ) * num_threads);

    for(int i = 0; i < num_threads; i++) {
        pthread_create(&pool.threads[i], NULL, thread_do_work, NULL);
    }
}

//Adiciona uma nova tarefa à fila de tarefas da pool
//Bloqueia e libera o mutex para garantir exclusão mútua
void thread_pool_add_task(void (*function)(void*), void* arg) {
    Task* new_task = malloc(sizeof(Task));
    new_task->function = function;
    new_task->arg = arg;
    new_task->next = NULL;

    pthread_mutex_lock(&pool.lock);
    if (pool.tasks_tail) {
        pool.tasks_tail->next = new_task;
        pool.tasks_tail = new_task;
    } else {
        pool.tasks_head = pool.tasks_tail = new_task;
    }
    pthread_cond_signal(&pool.notify);
    pthread_mutex_unlock(&pool.lock);
}

//Destrói a threadpool e libera os recursos
void thread_pool_destroy() {
    pthread_mutex_lock(&pool.lock);
    pool.shutdown = 1;
    pthread_cond_broadcast(&pool.notify);
    pthread_mutex_unlock(&pool.lock);

    for (int i = 0; i < pool.thread_count; i++) {
     pthread_join(pool.threads[i], NULL);
    }
    free(pool.threads);
    pthread_mutex_destroy(&pool.lock);
    pthread_cond_destroy(&pool.notify);
}
