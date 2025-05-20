#include "Enemy_manager.h"
#include "globals.h"
#include "Enemy.h"

Enemy_manager* Enemy_manager::instance = nullptr;

Enemy_manager& Enemy_manager::get_instance() {
    if (instance == nullptr) {
        instance = new Enemy_manager();
    }
    return *instance;
}

void Enemy_manager::update_all() {
    for (auto& enemy : enemies) {
        enemy.update();
    }
}

void Enemy_manager::spawn() {
    // Create enemies, incrementing their amount every time a new one is created
    enemies.clear();

    for (size_t row = 0; row < Level::get_rows(); ++row) {
        for (size_t column = 0; column < Level::get_columns(); ++column) {
            char cell = Level::get_cell(row, column);

            if (cell == ENEMY) {
                // Instantiate and add an enemy to the level
                enemies.push_back({
                        {static_cast<float>(column), static_cast<float>(row)},
                        true
                });

                Level::set_cell(row, column, AIR);
            }
        }
    }
}

// Custom is_colliding function for enemies
bool Enemy_manager::is_colliding_with(const Vector2& position) {
    Rectangle entity_hitbox = {position.x, position.y, 1.0f, 1.0f};
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        Rectangle enemy_hitbox = {(float) it->get_x(), (float) it->get_y(), 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}


void Enemy_manager::remove_colliding(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        Rectangle enemy_hitbox = {(float) it->get_x(), (float) it->get_y(), 1.0f, 1.0f};
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
