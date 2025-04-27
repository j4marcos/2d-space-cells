// fhysics.h
#ifndef FHYSICS_H
#define FHYSICS_H

// World bounds
#define LIMIT_X 1000
#define LIMIT_Y 1000

// Collision radius for objects (assumed circular)
#define COLLISION_RADIUS 10.0f  // antes era 5.0f


// Simulation object
typedef struct {
    float x, y;
    float vx, vy;
    int   type;
} Object;

// Handle collisions with world boundaries
void handle_collisions(Object* object);

// Resolve elastic collision between two objects in the same quadrant
void resolve_object_collision(Object* a, Object* b);

#endif // FHYSICS_H
