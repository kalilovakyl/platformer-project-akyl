#include "player.h"
#include "globals.h"

player c_player;

player::player() {}

void player::reset_stats() {
    lives = MAX_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        level_scores[i] = 0;
    }
}

void player::increment_score() {
    PlaySound(coin_sound);
    level_scores[level_index]++;
}

int player::get_total_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += level_scores[i];
    }

    return sum;
}

void player::spawn() {
    y_velocity = 0;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);;

            if (cell == PLAYER) {
                pos.x = column;
                pos.y = row;
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void player::kill() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
    level_scores[level_index] = 0;
}

void player::move_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = pos.x + delta;
    if (!is_colliding({next_x, pos.y}, WALL)) {
        pos.x = next_x;
    }
    else {
        pos.x = roundf(pos.x);
        return;
    }

    // For drawing player animations
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void player::update_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (is_colliding({pos.x, pos.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position
    pos.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_on_ground = is_colliding({pos.x, pos.y + 0.1f}, WALL);
    if (is_on_ground) {
        y_velocity = 0;
        pos.y = roundf(pos.y);
    }
}

void player::update_player() {
    update_gravity();

    // Interacting with other level elements
    if (is_colliding(pos, COIN)) {
        get_collider(pos, COIN) = AIR; // Removes the coin
        increment_score();
    }

    if (is_colliding(pos, EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (is_colliding(pos, SPIKE) || pos.y > current_level.rows) {
        kill();
    }

    // Upon colliding with an enemy...
    if (is_colliding_with_enemies(pos)) {
        // ...check if their velocity is downwards...
        if (y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            remove_colliding_enemy(pos);
            PlaySound(kill_enemy_sound);

            increment_score();
            y_velocity = -BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
            kill();
        }
    }
}

int player::get_lives() {
    return lives;
}

float player::get_x() {
    return pos.x;
}

float player::get_y() {
    return pos.y;
}

bool player::get_is_on_ground() {
    return is_on_ground;
}

bool player::get_is_looking_forward() {
    return is_looking_forward;
}
bool player::get_is_moving() {
    return is_moving;
};

void player::set_is_moving(bool statement) {
    is_moving = statement;
}

void player::set_is_on_ground(bool statement) {
    is_on_ground = statement;
}
void player::set_y_velocity(float value) {
    y_velocity = value;
}