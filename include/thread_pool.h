#ifndef THREAD_POOL_H
#define THREAD_POOL_H

//inicializa a thread pool, num_threads número de threads.
void thread_pool_init(int num_threads);

//adiciona nova tarefa à fila de tarefas da thread pool
void thread_pool_add_task(void (*function)(void*), void* arg);

//finaliza a thread pool. Encerra as threads e libera recursos
void thread_pool_destroy();

#endif
