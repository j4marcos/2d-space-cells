#ifndef FHYSICS_H
#define FHYSICS_H

//limites do mundo
#define LIMIT_X 1000
#define LIMIT_Y 1000

// Raio de colisão dos objetos
#define COLLISION_RADIUS 10.0f  

//estrutura dos objetos
typedef struct {
    float x, y; 
    float vx, vy; 
    int   type;
} Object;

// Trata colisões de objetos com os limites do mundo
void handle_collisions(Object* object);

//Resolve colisões entre dois objetos
void resolve_object_collision(Object* a, Object* b);

#endif // FHYSICS_H
