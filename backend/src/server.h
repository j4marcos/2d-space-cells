// backend/src/server.h
#ifndef SERVER_H
#define SERVER_H

#include "game.h"
#include <string>
#include <thread>
#include <mutex>
#include <set>
#include <map>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> ws_server;
typedef websocketpp::connection_hdl connection_hdl;

class Server {
public:
    Server(GameWorld* game);
    void run(uint16_t port);

private:
    GameWorld* game;
    ws_server server;
    std::mutex conns_mutex;
    std::map<connection_hdl, int, std::owner_less<connection_hdl>> connections;

    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, ws_server::message_ptr msg);

    void send_state_loop();
};

#endif // SERVER_H
