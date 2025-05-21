#include "Level.h"
#include "globals.h"

Level* Level::instance = nullptr;

std::vector<LevelData> levels = {
    { 12, 72, LEVEL_1_DATA },
    { 12, 78, LEVEL_2_DATA },
    { 12, 86, LEVEL_3_DATA }
};

Level& Level::get_instance() {
    if (instance == nullptr) {
        instance = new Level();
    }
    return *instance;
}

void Level::initialize(const std::vector<LevelData>& c_levels) {
    levels = c_levels;
    current_index = 0;
}

bool Level::is_inside(int row, int column) {
    if (row < 0 || row >= levels[current_index].rows) return false;
    if (column < 0 || column >= levels[current_index].columns) return false;
    return true;
}

bool Level::is_colliding(Vector2 pos, char look_for) {
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

char& Level::get_collider(Vector2 pos, char look_for) {
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

void Level::reset_index() {
    current_index = 0;
}

void Level::load(int offset) {
    current_index += offset;

    // Win logic
    if (current_index >= levels.size()) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        current_index = 0;
        return;
    }

    // Level duplication
    size_t rows = levels[current_index].rows;
    size_t columns = levels[current_index].columns;
    current_level_data = new char[rows*columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = levels[current_index].data[row * columns + column];
        }
    }

    // Instantiate entities
    Player::get_instance().spawn();
    Enemy_manager::get_instance().spawn();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void Level::unload() {
    delete[] current_level_data;
    current_level_data = nullptr;
}

// Getters and setters
char& Level::get_cell(size_t row, size_t column) {
    size_t rows = get_rows();
    size_t columns = get_columns();
    return current_level_data[row * levels[current_index].columns + column];
}

void Level::set_cell(size_t row, size_t column, char chr) {
    get_cell(row, column) = chr;
}

size_t Level::get_rows() {
    return levels[current_index].rows;
}

size_t Level::get_columns() {
    return levels[current_index].columns;
}
int Level::get_index() {
    if (current_index >= 0 && current_index < levels.size()) {
        return current_index;
    }
    return 0;
}