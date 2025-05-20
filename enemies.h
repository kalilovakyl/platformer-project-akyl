#ifndef ENEMIES_H
#define ENEMIES_H

#include "enemy.h"
#include <vector>
#include "raylib.h"

class enemies {
    std::vector<enemy> all_enemy;
public:
    enemies() {}

    void update_all();
    void spawn();
    bool is_colliding_with(const Vector2& pos);
    void remove_colliding(Vector2 pos);

    // getter
    const std::vector<enemy>& get_all_enemy() const { return all_enemy; }
};

extern enemies c_enemies;

#endif //ENEMIES_H
