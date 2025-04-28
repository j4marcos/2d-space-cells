#include "manager.h"
#include "inputs.h"
#include "worker.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "fhysics.h"
#include <math.h>

ObjectList* quadrant_lists = NULL;
double       total_update_time = 0;
pthread_t  threads[NUM_QUADRANTS];
ThreadData thread_data[NUM_QUADRANTS];
int total_objects = 0;
Object objects[MAX_OBJECTS];

// Initialize or resize quadrant lists
static void init_quadrants() {
        quadrant_lists = malloc(sizeof(ObjectList) * NUM_QUADRANTS);
        for (int i = 0; i < NUM_QUADRANTS; i++) {
            quadrant_lists[i].count    = 0;
    quadrant_lists[i].capacity = NUM_QUADRANTS;
       quadrant_lists[i].items    = malloc(sizeof(Object*) * quadrant_lists[i].capacity);
       }
    }

static void free_quadrants() {
    for (int i = 0; i < NUM_QUADRANTS; i++)
        free(quadrant_lists[i].items);
    free(quadrant_lists);
}

static void clear_quadrants() {
    for (int i = 0; i < NUM_QUADRANTS; i++)
        quadrant_lists[i].count = 0;
}

static void add_to_quadrant(int q, Object* obj) {
        ObjectList* L = &quadrant_lists[q];
        if (L->count == L->capacity) {
            L->capacity *= 2;
            L->items = realloc(L->items, sizeof(Object*) * L->capacity);
        }
        L->items[L->count++] = obj;
    }

void initialize_manager() {
    total_objects = 0;
    init_quadrants();
}

void update_simulation() {
    // Measure frame start
    double frame_start = (double)clock() / CLOCKS_PER_SEC;

    // Process new inputs
    NewObject new_objects[128];
    int new_count = get_new_objects(new_objects, 128);
    for (int i = 0; i < new_count && total_objects < MAX_OBJECTS; i++) {
        objects[total_objects++] = (Object){
            new_objects[i].x, new_objects[i].y,
            new_objects[i].vx, new_objects[i].vy,
            new_objects[i].type
        };
    }

    // Regroup objects into fixed quadrants
    clear_quadrants();
    int grid = (int) sqrt(NUM_QUADRANTS);
    float cell_w = LIMIT_X / (float)grid;
    float cell_h = LIMIT_Y / (float)grid;
    for (int i = 0; i < total_objects; i++) {
        Object* o = &objects[i];
        int gx = fmin(grid - 1, o->x / cell_w);
        int gy = fmin(grid - 1, o->y / cell_h);
        int q  = gy * grid + gx;
        add_to_quadrant(q, o); 
    }

    // Spawn threads for each quadrant
    for (int i = 0; i < NUM_QUADRANTS; i++) {
        thread_data[i].id              = i;
        thread_data[i].list            = &quadrant_lists[i];
        thread_data[i].processing_time = 0.0;
        pthread_create(&threads[i], NULL, process_quadrant, &thread_data[i]);
    }

    // Wait for all threads and compute total time
    for (int i = 0; i < NUM_QUADRANTS; i++) {
        pthread_join(threads[i], NULL);
    }
    double frame_end = (double)clock() / CLOCKS_PER_SEC;
    total_update_time = frame_end - frame_start;
}

void finalize_manager() {
    free_quadrants();
}
