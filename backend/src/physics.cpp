// backend/src/physics.cpp
#include "physics.h"
#include <cmath>

void update_physics(GameObject& obj, float dt) {
    obj.pos.x += obj.vel.x * dt;
    obj.pos.y += obj.vel.y * dt;
}

void update_player(Player& player, float dt) {
    const float thrust_force = 50.0f;
    const float angular_speed = 2.5f;

    if (player.turn_left) player.angle -= angular_speed * dt;
    if (player.turn_right) player.angle += angular_speed * dt;

    if (player.thrust) {
        player.vel.x += cos(player.angle) * thrust_force * dt;
        player.vel.y += sin(player.angle) * thrust_force * dt;
    }

    update_physics(player, dt);
}

bool check_collision(const GameObject& a, const GameObject& b) {
    float dx = a.pos.x - b.pos.x;
    float dy = a.pos.y - b.pos.y;
    float dist2 = dx * dx + dy * dy;
    float rsum = a.radius + b.radius;
    return dist2 <= rsum * rsum;
}

void resolve_collisions(Quadrant& quad) {
    // projetil vs asteroide
    for (auto& proj : quad.projectiles) {
        if (!proj->alive) continue;
        for (auto& ast : quad.asteroids) {
            if (check_collision(*proj, *ast)) {
                proj->alive = false;
                ast->vel.x += proj->vel.x * 0.1f;
                ast->vel.y += proj->vel.y * 0.1f;
                break;
            }
        }
    }
}
