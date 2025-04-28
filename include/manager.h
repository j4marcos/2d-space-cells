

// manager.h
#ifndef MANAGER_H
#define MANAGER_H

#include "fhysics.h"
#include <time.h>
#include <pthread.h>

#define MAX_OBJECTS 4096
#ifndef NUM_QUADRANTS
#define NUM_QUADRANTS 4 // mudar para 1, 4, ou 16
#endif

// Dynamic array of Objects for each quadrant
 typedef struct {
       Object** items;   // array de ponteiros para Object
         int       count;
         int       capacity;
     } ObjectList;

// Data passed to each thread for processing its quadrant
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

void initialize_manager();
void update_simulation();
void finalize_manager();

#endif // MANAGER_H