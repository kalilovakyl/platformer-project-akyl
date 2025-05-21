#include "Player.h"
#include "globals.h"

Player* Player::instance = nullptr;

Player& Player::get_instance() {
    if (instance == nullptr) {
        instance = new Player();
    }
    return *instance;
}

void Player::reset_stats() {
    lives = MAX_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        level_scores[i] = 0;
    }
}

void Player::increment_score() {
    PlaySound(coin_sound);
    level_scores[Level::get_instance().get_index()]++;
}

int Player::get_total_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += level_scores[i];
    }

    return sum;
}

void Player::spawn() {
    y_velocity = 0;

    for (size_t row = 0; row < Level::get_instance().get_rows(); ++row) {
        for (size_t column = 0; column < Level::get_instance().get_columns(); ++column) {
            char cell = Level::get_instance().get_cell(row, column);;

            if (cell == PLAYER) {
                pos.x = column;
                pos.y = row;
                Level::get_instance().set_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
    level_scores[Level::get_instance().get_index()] = 0;
}

void Player::move_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = pos.x + delta;
    if (!Level::get_instance().is_colliding({next_x, pos.y}, WALL)) {
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

void Player::update_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (Level::get_instance().is_colliding({pos.x, pos.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position
    pos.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_on_ground = Level::get_instance().is_colliding({pos.x, pos.y + 0.1f}, WALL);
    if (is_on_ground) {
        y_velocity = 0;
        pos.y = roundf(pos.y);
    }
}

void Player::update() {
    update_gravity();

    // Interacting with other level elements
    if (Level::get_instance().is_colliding(pos, COIN)) {
        Level::get_instance().get_collider(pos, COIN) = AIR; // Removes the coin
        increment_score();
    }

    if (Level::get_instance().is_colliding(pos, EXIT)) {
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
            Level::get_instance().load(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (Level::get_instance().is_colliding(pos, SPIKE) || pos.y > Level::get_instance().get_rows()) {
        kill();
    }

    // Upon colliding with an enemy...
    if (Enemy_manager::get_instance().is_colliding_with(pos)) {
        // ...check if their velocity is downwards...
        if (y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            Enemy_manager::get_instance().remove_colliding(pos);
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

int Player::get_lives() {
    return lives;
}

float Player::get_x() {
    return pos.x;
}

float Player::get_y() {
    return pos.y;
}

bool Player::get_is_on_ground() {
    return is_on_ground;
}

bool Player::get_is_looking_forward() {
    return is_looking_forward;
}
bool Player::get_is_moving() {
    return is_moving;
};

void Player::set_is_moving(bool statement) {
    is_moving = statement;
}

void Player::set_is_on_ground(bool statement) {
    is_on_ground = statement;
}
void Player::set_y_velocity(float value) {
    y_velocity = value;
}