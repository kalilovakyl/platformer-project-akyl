#ifndef ENEMIES_H
#define ENEMIES_H

#include "Enemy.h"
#include <vector>
#include "raylib.h"

class Enemy_manager {
    std::vector<Enemy> all_enemy;
public:
    Enemy_manager() {}

    void update_all();
    void spawn();
    bool is_colliding_with(const Vector2& pos);
    void remove_colliding(Vector2 pos);

    // getter
    const std::vector<Enemy>& get_all_enemy() const { return all_enemy; }
};

extern Enemy_manager c_enemies;

#endif //ENEMIES_H
