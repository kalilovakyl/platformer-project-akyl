#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "vector"

struct LevelData {
    size_t rows;
    size_t columns;
    const char* data;

    LevelData(size_t c_rows, size_t c_columns, char* c_data)
        : rows(c_rows), columns(c_columns), data(c_data) {}
};

class Level {
    Level() {}

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    static Level* instance;

    int current_index = 0;
    std::vector<LevelData> levels;
    char* current_level_data = nullptr;
public:
    static Level& get_instance();

    void initialize(const std::vector<LevelData>& c_levels);

    bool is_inside(int row, int column);
    bool is_colliding(Vector2 pos, char look_for);
    char& get_collider(Vector2 pos, char look_for);

    void reset_index();
    void load(int offset);
    void unload();

    // Getters and setters
    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char chr);

    size_t get_rows();
    size_t get_columns();
    int get_index();
};

#endif //LEVEL_H
