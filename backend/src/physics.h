// backend/src/physics.h
#ifndef PHYSICS_H
#define PHYSICS_H

#include "game.h"

void update_physics(GameObject& obj, float dt);
void update_player(Player& player, float dt);
void resolve_collisions(Quadrant& quad);
bool check_collision(const GameObject& a, const GameObject& b);

#endif // PHYSICS_H
