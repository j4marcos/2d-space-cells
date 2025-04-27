// worker.h
#ifndef WORKER_H
#define WORKER_H

#include "manager.h"  // for ThreadData and ObjectList definitions

// Entry point for quadrant processing threads
void* process_quadrant(void* arg);

#endif // WORKER_H