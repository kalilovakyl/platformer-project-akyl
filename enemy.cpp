#include "enemy.h"
#include "globals.h"
#include "raylib.h"

enemy c_enemy({0.0f, 0.0f}, true);

void enemy::spawn() {
    // Create enemies, incrementing their amount every time a new one is created
    enemies.clear();

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);

            if (cell == ENEMY) {
                // Instantiate and add an enemy to the level
                enemies.push_back({
                        {static_cast<float>(column), static_cast<float>(row)},
                        true
                });

                set_level_cell(row, column, AIR);
            }
        }
    }
}

void enemy::update() {
    for (auto &enemy : enemies) {
        // Find the enemy's next x
        float next_x = enemy.pos.x;
        next_x += (enemy.is_looking_right ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);

        // If its next position collides with a wall, turn around
        if (is_colliding({next_x, enemy.pos.y}, WALL)) {
            enemy.is_looking_right = !enemy.is_looking_right;
        }
        // Otherwise, keep moving
        else {
            enemy.pos.x = next_x;
        }
    }
}

// Custom is_colliding function for enemies
bool enemy::is_colliding_with(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto &enemy : enemies) {
        Rectangle enemy_hitbox = {(float) enemy.pos.x, (float) enemy.pos.y, 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}

void enemy::remove_colliding(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        Rectangle enemy_hitbox = {(float) it->pos.x, (float) it->pos.y, 1.0f, 1.0f};
        // Erase a colliding enemy
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            enemies.erase(it);

            // Call the function again to remove any remaining colliding enemies
            // Cannot continue as calling erase on a vector invalidates current iterators
            remove_colliding(pos);
            return;
        }
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

const std::vector<enemy>& enemy::get_enemies() const {
    return enemies;
}