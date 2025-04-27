

// server.c
#include "server.h"
#include "inputs.h"
#include "manager.h"
#include "worker.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fhysics.h>

#define BUF_SIZE 4096

extern int total_objects;
extern Object objects[];
extern double total_update_time;
extern ThreadData thread_data[NUM_QUADRANTS];

// FPS variables
 int frames = 0;
 double last_fps_time = 0;
 int current_fps = 0;

void update_fps() {
    double now = (double)clock() / CLOCKS_PER_SEC;
    frames++;
    if (now - last_fps_time >= 1.0) {
        current_fps = frames;
        frames = 0;
        last_fps_time = now;
    }
}

void serve_file(int client_fd, const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        const char* error = "HTTP/1.1 404 Not Found\r\n\r\n404 Not Found";
        send(client_fd, error, strlen(error), 0);
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char header[256];
    const char* type = strstr(path, ".js") ? "application/javascript" :
                       strstr(path, ".css") ? "text/css" :
                       "text/html";

    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n\r\n", type, size);

    send(client_fd, header, strlen(header), 0);

    char buffer[BUF_SIZE];
    size_t read_bytes;
    while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_fd, buffer, read_bytes, 0);
    }
    fclose(file);
}

void* handle_connection(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[BUF_SIZE];
    int received = recv(client_socket, buffer, BUF_SIZE - 1, 0);
    if (received <= 0) {
        close(client_socket);
        return NULL;
    }

    buffer[received] = '\0';

    if (strncmp(buffer, "GET /state", 10) == 0) {
        update_fps();
        char response[BUF_SIZE * 4];
        int pos = 0;

        pos += sprintf(response + pos,
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
        pos += sprintf(response + pos, "{");
        pos += sprintf(response + pos, "\"fps\":%d,", current_fps);
        pos += sprintf(response + pos, "\"total_objects\":%d,", total_objects);
        pos += sprintf(response + pos, "\"thread_count\":%d,", NUM_QUADRANTS);

        pos += sprintf(response + pos, "\"thread_times\":[");
        for (int i = 0; i < NUM_QUADRANTS; i++) {
            pos += sprintf(response + pos, "%.6f", thread_data[i].processing_time);
            if (i < NUM_QUADRANTS - 1) pos += sprintf(response + pos, ",");
        }
        pos += sprintf(response + pos, "],");

        pos += sprintf(response + pos, "\"update_time\":%.6f,", total_update_time);
        pos += sprintf(response + pos, "\"objects\":[");
        for (int i = 0; i < total_objects; i++) {
            pos += sprintf(response + pos,
                "{\"x\":%.2f,\"y\":%.2f,\"vx\":%.2f,\"vy\":%.2f,\"type\":%d}",
                objects[i].x, objects[i].y,
                objects[i].vx, objects[i].vy,
                objects[i].type);
            if (i < total_objects - 1) pos += sprintf(response + pos, ",");
        }
        pos += sprintf(response + pos, "]}");

        send(client_socket, response, pos, 0);

    } else if (strncmp(buffer, "POST /input", 11) == 0) {
        char* body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4;
            float x, y, vx, vy;
            int type;
            if (sscanf(body, "x=%f&y=%f&vx=%f&vy=%f&type=%d", &x, &y, &vx, &vy, &type) == 5) {
                add_new_object(x, y, vx, vy, type);
            }
        }
        const char* ok = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nOK";
        send(client_socket, ok, strlen(ok), 0);

    } else {
        if (strncmp(buffer, "GET /", 5) == 0) {
            char path[256] = "public";
            char* start = buffer + 4;
            char* end = strchr(start, ' ');
            if (end) *end = '\0';
            if (strlen(start) == 0 || strcmp(start, "/") == 0)
                strcat(path, "/index.html");
            else
                strcat(path, start);
            serve_file(client_socket, path);
        } else {
            const char* nf = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot found";
            send(client_socket, nf, strlen(nf), 0);
        }
    }

    close(client_socket);
    return NULL;
}

void* start_http_server(void* arg) {
    int port = *(int*)arg;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) { perror("socket"); exit(1); }
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) { perror("bind"); exit(1); }
    if (listen(server_socket, 10) < 0) { perror("listen"); exit(1); }
    printf("[SERVER] HTTP running on port %d\n", port);
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int* new_socket = malloc(sizeof(int));
        *new_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
        if (*new_socket < 0) { perror("accept"); free(new_socket); continue; }
        pthread_t thread;
        pthread_create(&thread, NULL, handle_connection, new_socket);
        pthread_detach(thread);
    }
    return NULL;
}
