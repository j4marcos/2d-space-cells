// backend/src/server.cpp
#include "server.h"
#include <websocketpp/common/thread.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

Server::Server(GameWorld* game) : game(game) {
    server.clear_access_channels(websocketpp::log::alevel::all);  // <== remove [frame_payload]
    server.init_asio();
    server.set_open_handler([this](connection_hdl hdl) { on_open(hdl); });
    server.set_close_handler([this](connection_hdl hdl) { on_close(hdl); });
    server.set_message_handler([this](connection_hdl hdl, ws_server::message_ptr msg) {
        on_message(hdl, msg);
    });
}

void Server::run(uint16_t port) {
    server.listen(port);
    server.start_accept();
    std::thread([this]() { send_state_loop(); }).detach();
    server.run();
}

void Server::on_open(connection_hdl hdl) {
    std::cout << "Nova conexão estabelecida.\n";

    // Verifica se o jogador já está registrado nesta conexão
    std::lock_guard<std::mutex> lock(conns_mutex);
    if (connections.count(hdl) > 0) {
        std::cerr << "Conexão já registrada. Ignorando.\n";
        return;
    }

    // Cria novo jogador se não existir
    auto player = game->add_player();
    connections[hdl] = player->id;

    json response = { {"type", "register"}, {"player_id", player->id} };
    server.send(hdl, response.dump(), websocketpp::frame::opcode::text);
    std::cout << "Player " << player->id << " conectado.\n";
}

void Server::on_close(connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(conns_mutex);

    // Verificar se a conexão está registrada
    if (connections.count(hdl) > 0) {
        int player_id = connections[hdl];

        // Remover o jogador do jogo
        game->remove_player(player_id);  // Adicionar função de remoção no game

        // Remover a conexão
        connections.erase(hdl);

        std::cout << "Conexao encerrada. Jogador " << player_id << " removido.\n";
    }
}

void Server::on_message(connection_hdl hdl, ws_server::message_ptr msg) {
    int player_id;
    {
        std::lock_guard<std::mutex> lock(conns_mutex);
        if (connections.count(hdl) == 0) return;
        player_id = connections[hdl];
    }

    try {
        auto data = json::parse(msg->get_payload());
        if (data["type"] == "command") {
            bool thrust = data.value("thrust", false);
            bool left = data.value("left", false);
            bool right = data.value("right", false);
            bool shoot = data.value("shoot", false);
            game->apply_command(player_id, thrust, left, right, shoot);
        }
    } catch (...) {
        std::cerr << "Mensagem inválida recebida.\n";
    }
}

void Server::send_state_loop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::string state = game->get_state_json();

        std::lock_guard<std::mutex> lock(conns_mutex);
        for (auto it = connections.begin(); it != connections.end(); ) {
            try {
                server.send(it->first, state, websocketpp::frame::opcode::text);
                ++it;
            } catch (const websocketpp::exception& e) {
                std::cerr << "[send_state_loop] erro ao enviar: " << e.what() << "\n";
                it = connections.erase(it); // remove conexão inválida
            }
        }
    }
}
