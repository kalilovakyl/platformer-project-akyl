#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "vector"

class level {
    size_t rows = 0, columns = 0;
    char *data = nullptr;
    static int index;

    static std::vector<level> all;
    static char* level_data;
public:
    level(size_t c_rows, size_t c_columns, char *c_data)
        : rows(c_rows), columns(c_columns), data(c_data) {}

    static bool is_inside(int row, int column);
    static bool is_colliding(Vector2 pos, char look_for);
    static char& get_collider(Vector2 pos, char look_for);

    static void reset_index();
    static void load(int offset);
    static void unload();

    // Getters and setters
    static char& get_cell(size_t row, size_t column);
    static void set_cell(size_t row, size_t column, char chr);

    static size_t get_rows();
    static size_t get_columns();
    static int get_index();
};

extern level LEVEL_1;
extern level LEVEL_2;
extern level LEVEL_3;

#endif //LEVEL_H
