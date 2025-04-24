// backend/src/game.h
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <mutex>
#include <unordered_map>
#include <memory>

struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
};

struct GameObject {
    int id;
    Vec2 pos, vel;
    float radius;
    bool alive;

    GameObject(int id, Vec2 pos, Vec2 vel, float radius)
        : id(id), pos(pos), vel(vel), radius(radius), alive(true) {}

    virtual ~GameObject() {}
};

struct Projectile : public GameObject {
    int owner_id;
    float ttl; // tempo de vida

    Projectile(int id, Vec2 pos, Vec2 vel, float radius, int owner_id, float ttl)
        : GameObject(id, pos, vel, radius), owner_id(owner_id), ttl(ttl) {}
};

struct Asteroid : public GameObject {
    Asteroid(int id, Vec2 pos, Vec2 vel, float radius)
        : GameObject(id, pos, vel, radius) {}
};

struct Player : public GameObject {
    float angle;
    bool thrust, turn_left, turn_right, shoot;

    Player(int id, Vec2 pos)
        : GameObject(id, pos, Vec2(0, 0), 15.0f), angle(0),
          thrust(false), turn_left(false), turn_right(false), shoot(false) {}
};

struct Quadrant {
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Asteroid>> asteroids;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::mutex mutex;
};

class GameWorld {
public:
    static const int num_quadrants = 4;
    Quadrant quadrants[num_quadrants];
    int next_id = 1;

    void init();
    void simulate_quadrant(int id);
    std::shared_ptr<Player> add_player();
    void apply_command(int player_id, bool thrust, bool left, bool right, bool shoot);
    std::string get_state_json();
    void remove_player(int player_id);
    

private:
    std::unordered_map<int, std::shared_ptr<Player>> all_players;
    std::mutex global_mutex;
};

#endif // GAME_H
