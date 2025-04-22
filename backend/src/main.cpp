// backend/src/main.cpp
#include "game.h"
#include "server.h"
#include <thread>
#include <vector>

int main() {
    // Inicializa mundo do jogo
    GameWorld game;
    game.init();

    // Inicializa servidor WebSocket
    Server server(&game);
    std::thread server_thread([&]() {
        server.run(9002); // Porta WebSocket
    });

    // Inicializa threads de física (um por quadrante)
    std::vector<std::thread> physics_threads;
    for (int i = 0; i < game.num_quadrants; ++i) {
        physics_threads.emplace_back([&, i]() {
            game.simulate_quadrant(i);
        });
    }

    // Espera finalização
    server_thread.join();
    for (auto& t : physics_threads) t.join();

    return 0;
}
