#include "enemies.h"
#include "globals.h"
#include "enemy.h"

enemies c_enemies;

void enemies::update_all() {
    for (auto& e : all_enemy) {
        e.update();
    }
}

void enemies::spawn() {
    // Create enemies, incrementing their amount every time a new one is created
    all_enemy.clear();

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);

            if (cell == ENEMY) {
                // Instantiate and add an enemy to the level
                all_enemy.push_back({
                        {static_cast<float>(column), static_cast<float>(row)},
                        true
                });

                set_level_cell(row, column, AIR);
            }
        }
    }
}

// Custom is_colliding function for enemies
bool enemies::is_colliding_with(const Vector2& position) {
    Rectangle entity_hitbox = {position.x, position.y, 1.0f, 1.0f};
    for (auto it = all_enemy.begin(); it != all_enemy.end(); it++) {
        Rectangle enemy_hitbox = {(float) it->get_x(), (float) it->get_y(), 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}


void enemies::remove_colliding(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto it = all_enemy.begin(); it != all_enemy.end(); it++) {
        Rectangle enemy_hitbox = {(float) it->get_x(), (float) it->get_y(), 1.0f, 1.0f};
        // Erase a colliding enemy
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            all_enemy.erase(it);

            // Call the function again to remove any remaining colliding enemies
            // Cannot continue as calling erase on a vector invalidates current iterators
            remove_colliding(pos);
            return;
        }
    }
}
