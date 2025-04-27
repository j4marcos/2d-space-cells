// main.c
#include "inputs.h"
#include "manager.h"
#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int main() {
    initialize_inputs();
    initialize_manager();

    pthread_t server_thread;
    int port = PORT;
    pthread_create(&server_thread, NULL, (void*(*)(void*))start_http_server, &port);
    
    printf("Simulation started!\n");
    
    // TEST: create some initial objects
    add_new_object(100, 100, 5, 3, 0);
    add_new_object(200, 300, -2, 4, 1);
    add_new_object(400, 500, 1, -3, 0);
    
    while (1) {
        update_simulation();
        usleep(16000); // Approximately 60 FPS (16ms)
    }

    finalize_manager();
    return 0;
}