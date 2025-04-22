// backend/src/game.cpp
#include "game.h"
#include "physics.h"
#include <cmath>
#include <chrono>
#include <thread>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void GameWorld::init() {
    // Criação de alguns asteroides iniciais em cada quadrante
    for (int q = 0; q < num_quadrants; ++q) {
        auto& quad = quadrants[q];
        for (int i = 0; i < 3; ++i) {
            Vec2 pos((q % 2) * 500 + rand() % 200, (q / 2) * 500 + rand() % 200);
            Vec2 vel((rand() % 100 - 50) / 50.0f, (rand() % 100 - 50) / 50.0f);
            quad.asteroids.emplace_back(std::make_shared<Asteroid>(next_id++, pos, vel, 30.0f));
        }
    }
}

void GameWorld::simulate_quadrant(int id) {
    auto& quad = quadrants[id];
    const float dt = 0.016f; // ~60 FPS

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        std::lock_guard<std::mutex> lock(quad.mutex);

        for (auto& player : quad.players) {
            update_player(*player, dt);
        }
        for (auto& asteroid : quad.asteroids) {
            update_physics(*asteroid, dt);
        }
        for (auto& proj : quad.projectiles) {
            update_physics(*proj, dt);
            proj->ttl -= dt;
            if (proj->ttl <= 0) proj->alive = false;
        }
        resolve_collisions(quad);

        // Limpa projéteis mortos
        quad.projectiles.erase(std::remove_if(quad.projectiles.begin(), quad.projectiles.end(),
            [](auto p){ return !p->alive; }), quad.projectiles.end());
    }
}

std::shared_ptr<Player> GameWorld::add_player() {
    std::lock_guard<std::mutex> lock(global_mutex);
    Vec2 pos(250, 250);
    auto p = std::make_shared<Player>(next_id++, pos);
    all_players[p->id] = p;
    quadrants[0].players.push_back(p); // provisoriamente quadrante 0
    return p;
}

void GameWorld::apply_command(int player_id, bool thrust, bool left, bool right, bool shoot) {
    std::lock_guard<std::mutex> lock(global_mutex);
    if (all_players.count(player_id)) {
        auto& p = all_players[player_id];
        p->thrust = thrust;
        p->turn_left = left;
        p->turn_right = right;
        if (shoot && p->alive) {
            Vec2 dir(cos(p->angle), sin(p->angle));
            Vec2 proj_vel = { p->vel.x + dir.x * 5.0f, p->vel.y + dir.y * 5.0f };
            Vec2 proj_pos = { p->pos.x + dir.x * p->radius, p->pos.y + dir.y * p->radius };
            auto proj = std::make_shared<Projectile>(next_id++, proj_pos, proj_vel, 3.0f, p->id, 3.0f);
            quadrants[0].projectiles.push_back(proj); // todo: checar quadrante correto
        }
        p->shoot = shoot;
    }
}

std::string GameWorld::get_state_json() {
    json j;
    for (auto& [id, p] : all_players) {
        j["players"].push_back({{"id", p->id}, {"x", p->pos.x}, {"y", p->pos.y}, {"angle", p->angle}});
    }
    for (int i = 0; i < num_quadrants; ++i) {
        std::lock_guard<std::mutex> lock(quadrants[i].mutex);
        for (auto& a : quadrants[i].asteroids) {
            j["asteroids"].push_back({{"x", a->pos.x}, {"y", a->pos.y}, {"r", a->radius}});
        }
        for (auto& pr : quadrants[i].projectiles) {
            j["projectiles"].push_back({{"x", pr->pos.x}, {"y", pr->pos.y}});
        }
    }
    return j.dump();
}
