#include "enemy.h"
#include "globals.h"
#include "raylib.h"

void enemy::update() {
    // Find the enemy's next x
    float next_x = pos.x;
    next_x += (is_looking_right ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);

    // If its next position collides with a wall, turn around
    if (is_colliding({next_x, pos.y}, WALL)) {
        is_looking_right = !is_looking_right;
    }
    // Otherwise, keep moving
    else {
        pos.x = next_x;
    }
}


void enemy::set_x(float value) {
    pos.x = value;
}

void enemy::set_y(float value) {
    pos.y = value;
}

float enemy::get_x() const {
    return pos.x;
}

float enemy::get_y() const {
    return pos.y;
}