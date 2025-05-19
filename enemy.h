#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <vector>

class enemy {
    Vector2 pos;
    bool is_looking_right;
    std::vector<enemy> enemies;
public:
    enemy(const Vector2& c_pos, bool c_is_looking_right = true)
    : pos(c_pos), is_looking_right(c_is_looking_right) {};

    void spawn();

    void update();

    // Custom is_colliding function for enemies
    bool is_colliding_with(Vector2 pos);
    void remove_colliding(Vector2 pos);

    // setters and getters
    void set_x(float);
    void set_y(float);

    float get_x() const;
    float get_y() const;
    const std::vector<enemy>& get_enemies() const;
};

extern enemy c_enemy;

#endif //ENEMY_H
