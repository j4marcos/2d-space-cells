
//Processa objetos em cada quadrante
#include "worker.h"
#include <time.h>
#include "fhysics.h"
//Processa os objetos de um quadrante
void* process_quadrant(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double start = (double)clock() / CLOCKS_PER_SEC;

     ObjectList* L = data->list;


     for (int i = 0; i < L->count; i++) {
         Object* o = L->items[i];      // agora: o ponteiro ao objeto original
        
         //Atualiza posição do objeto
        o->x += o->vx;
        o->y += o->vy;

        //Trata colisões com o limite da simulação
        handle_collisions(o);

        //Trata colisões entre objetos de um mesmo quadrante
        for (int j = i + 1; j < L->count; j++) {
            resolve_object_collision(o, L->items[j]);
        }
        
    }

    double end = (double)clock() / CLOCKS_PER_SEC;
    data->processing_time = end - start;

    return NULL;
}