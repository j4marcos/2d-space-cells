// inputs.h
#ifndef INPUTS_H
#define INPUTS_H

// Structure for new object input (0=circle,1=square)
typedef struct {
    float x, y;
    float vx, vy;
    int   type;
} NewObject;

// Initialize input queue
void initialize_inputs();
// Retrieve up to max_objs new objects; clears queue
int  get_new_objects(NewObject* buffer, int max_objs);
// Enqueue a new object for simulation
void add_new_object(float x, float y, float vx, float vy, int type);

#endif // INPUTS_H