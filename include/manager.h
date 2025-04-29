
#ifndef MANAGER_H
#define MANAGER_H

#include "fhysics.h"
#include <time.h>
#include <pthread.h>

//numero máximo de objetos
#define MAX_OBJECTS 4096

#ifndef NUM_QUADRANTS
#define NUM_QUADRANTS 4 //pode ser mudado para 1, 4, ou 16
#endif

//estrutura para armazenar objetos em cada quadrante
 typedef struct {
       Object** items;   // array de ponteiros para Object
         int       count;
         int       capacity;
     } ObjectList;

//Estrutura para armazenar dados de cada thread, cada thread processa um quadrante
typedef struct {
    int          id;
    ObjectList*  list;
    double       processing_time;
} ThreadData;

extern ObjectList* quadrant_lists;
extern double       total_update_time;
extern int          total_objects;
extern Object       objects[MAX_OBJECTS];
extern pthread_t    threads[NUM_QUADRANTS];
extern ThreadData   thread_data[NUM_QUADRANTS];

//inicializa o gerenciador da simulação
void initialize_manager();

//atualiza estado da simulação
void update_simulation();

//Termina o gerenciador e libera os recursos
void finalize_manager();

#endif // MANAGER_H