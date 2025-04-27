
// worker.c
#include "worker.h"
#include <time.h>
#include "fhysics.h"

void* process_quadrant(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double start = (double)clock() / CLOCKS_PER_SEC;

     ObjectList* L = data->list;
     for (int i = 0; i < L->count; i++) {
         Object* o = L->items[i];      // agora: o ponteiro ao objeto original
        // Update position
        o->x += o->vx;
        o->y += o->vy;
        // World-boundary collision
        handle_collisions(o);
        // Object-object collisions within this quadrant
        for (int j = i + 1; j < L->count; j++) {
            resolve_object_collision(o, L->items[j]);
        }
    }

    double end = (double)clock() / CLOCKS_PER_SEC;
    data->processing_time = end - start;

    return NULL;
}