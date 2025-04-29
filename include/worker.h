// worker.h
#ifndef WORKER_H
#define WORKER_H

#include "manager.h"

//função de entrada para threads que processam quadrantes
void* process_quadrant(void* arg);

#endif // WORKER_H