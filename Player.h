#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

const int LEVEL_COUNT = 3;

class Player {
    Player() {}

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    static Player* instance;

    float y_velocity = 0;
    Vector2 pos;

    bool is_on_ground = true;
    bool is_looking_forward = true;
    bool is_moving = false;

    int level_scores[LEVEL_COUNT] = {0, 0, 0};

    const int MAX_LIVES = 5;
    int lives = MAX_LIVES;
public:
    static Player& get_instance();

    void reset_stats();
    void increment_score();
    int get_total_score();
    void spawn();
    void kill();
    void move_horizontally(float delta);
    void update_gravity();
    void update();

    // getters and setters
    int get_lives();
    float get_x();
    float get_y();
    bool get_is_on_ground();
    bool get_is_looking_forward();
    bool get_is_moving();

    void set_is_moving(bool);
    void set_is_on_ground(bool);
    void set_y_velocity(float);
};

#endif //PLAYER_H
