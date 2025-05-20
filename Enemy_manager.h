#ifndef ENEMIES_H
#define ENEMIES_H

#include "Enemy.h"
#include <vector>
#include "raylib.h"

class Enemy_manager {
    Enemy_manager() {}

    Enemy_manager(const Enemy_manager&) = delete;
    Enemy_manager& operator=(const Enemy_manager&) = delete;

    static Enemy_manager* instance;

    std::vector<Enemy> enemies;
public:
    static Enemy_manager& get_instance();

    void update_all();
    void spawn();
    bool is_colliding_with(const Vector2& pos);
    void remove_colliding(Vector2 pos);

    // getter
    const std::vector<Enemy>& get_enemies() const { return enemies; }
};

#endif //ENEMIES_H
