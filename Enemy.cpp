#include "Enemy.h"
#include "globals.h"
#include "raylib.h"

void Enemy::update() {
    // Find the enemy's next x
    float next_x = pos.x;
    next_x += (is_looking_right ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);

    // If its next position collides with a wall, turn around
    if (Level::get_instance().is_colliding({next_x, pos.y}, WALL)) {
        is_looking_right = !is_looking_right;
    }
    // Otherwise, keep moving
    else {
        pos.x = next_x;
    }
}


void Enemy::set_x(float value) {
    pos.x = value;
}

void Enemy::set_y(float value) {
    pos.y = value;
}

float Enemy::get_x() const {
    return pos.x;
}

float Enemy::get_y() const {
    return pos.y;
}