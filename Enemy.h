#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <vector>

class Enemy {
    Vector2 pos;
    bool is_looking_right;
public:
    Enemy(const Vector2& c_pos, bool c_is_looking_right = true)
    : pos(c_pos), is_looking_right(c_is_looking_right) {};

    void update();

    // setters and getters
    void set_x(float);
    void set_y(float);

    float get_x() const;
    float get_y() const;

};

#endif //ENEMY_H
