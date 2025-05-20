#include "level.h"
#include "globals.h"

level LEVEL_1 = { 12, 72, LEVEL_1_DATA };
level LEVEL_2 = { 12, 78, LEVEL_2_DATA };
level LEVEL_3 = { 12, 86, LEVEL_3_DATA };

std::vector<level> level::all = {
    LEVEL_1, LEVEL_2, LEVEL_3
};

int level::index = 0;
char* level::level_data = nullptr;

bool level::is_inside(int row, int column) {
    if (row < 0 || row >= all[index].rows) return false;
    if (column < 0 || column >= all[index].columns) return false;
    return true;
}

bool level::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside(row, column)) continue;
            if (get_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& level::get_collider(Vector2 pos, char look_for) {
    // Like is_colliding(), except returns a reference to the colliding object
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside(row, column)) continue;
            if (get_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_cell(pos.x, pos.y);;
}

void level::reset_index() {
    index = 0;
}

void level::load(int offset) {
    if (level_data != nullptr) {
        unload();
    }

    index += offset;

    // Win logic
    if (index >= all.size()) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        index = 0;
        return;
    }

    // Level duplication
    size_t rows = all[index].rows;
    size_t columns = all[index].columns;
    level_data = new char[rows*columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            level_data[row * columns + column] = all[index].data[row * columns + column];
        }
    }

    // Instantiate entities
    c_player.spawn();
    c_enemies.spawn();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void level::unload() {
    delete[] level_data;
}

// Getters and setters
char& level::get_cell(size_t row, size_t column) {
    return level_data[row * all[index].columns + column];
}

void level::set_cell(size_t row, size_t column, char chr) {
    get_cell(row, column) = chr;
}

size_t level::get_rows() {
    return all[index].rows;
}

size_t level::get_columns() {
    return all[index].columns;
}
int level::get_index() {
    if (index >= 0 && index < all.size()) {
        return index;
    }
    return 0;
}