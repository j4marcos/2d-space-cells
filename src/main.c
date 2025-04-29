// main.c
#include "inputs.h"
#include "manager.h"
#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int main() {
    // Inicializa o sistema
    initialize_inputs();
    initialize_manager();

    //Inicia o servidor HTTP em uma thread separada
    pthread_t server_thread;
    int port = PORT;
    pthread_create(&server_thread, NULL, (void*(*)(void*))start_http_server, &port);
    
    printf("Simulation started!\n");
    
    //Objetos iniciais da simulação
    add_new_object(100, 100, 5, 3, 0);
    add_new_object(200, 300, -2, 4, 1);
    add_new_object(400, 500, 1, -3, 0);

    //Loop da simulação 
    while (1) {
        update_simulation();
        usleep(16000);    // Aproximadamente 60 fps

    }

    finalize_manager();
    return 0;
}