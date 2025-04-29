// inputs.h
#ifndef INPUTS_H
#define INPUTS_H

// Estrutura para representar entrada de novo objeto
typedef struct {
    float x, y; 
    float vx, vy; 
    int   type;
} NewObject;

//inicializa a fila de entrada para novos objetos
void initialize_inputs();

//Recupera objetos da fila e limpa a fila pós leitura
int  get_new_objects(NewObject* buffer, int max_objs);

//Adiciona um novo objeto à fila de entrada
void add_new_object(float x, float y, float vx, float vy, int type);

#endif // INPUTS_H