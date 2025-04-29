
#include "fhysics.h"
#include <math.h>

//Trata colisões entre objetos e o limite do mundo
void handle_collisions(Object* object) {
    if (object->x < 0) {
        object->x = 0;
        object->vx = -object->vx;
    } else if (object->x > LIMIT_X) {
        object->x = LIMIT_X;
        object->vx = -object->vx;
    }

    if (object->y < 0) {
        object->y = 0;
        object->vy = -object->vy;
    } else if (object->y > LIMIT_Y) {
        object->y = LIMIT_Y;
        object->vy = -object->vy;
    }
}
//Resolve colisões entre objetos 
void resolve_object_collision(Object* a, Object* b) {
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    float dist2 = dx*dx + dy*dy;
    float minDist = COLLISION_RADIUS * 2;

    if (dist2 <= minDist * minDist) {
        // Troca velocidades entre objetos (colisão elástica)
        float tvx = a->vx;
        float tvy = a->vy;
        a->vx = b->vx;
        a->vy = b->vy;
        b->vx = tvx;
        b->vy = tvy;

        // Separate overlapping objects
        float dist = sqrt(dist2);
        if (dist > 0.0f) {
            float overlap = (minDist - dist) / 2.0f;
            float ux = dx / dist;
            float uy = dy / dist;
            a->x -= ux * overlap;
            a->y -= uy * overlap;
            b->x += ux * overlap;
            b->y += uy * overlap;
        }
    }
}
