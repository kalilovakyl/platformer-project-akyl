#include "raylib.h"

#include "globals.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

void update_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = GAME_STATE;
                Level::load(0);
            }
            break;

        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                Player::get_instance().move_horizontally(PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                Player::get_instance().move_horizontally(-PLAYER_MOVEMENT_SPEED);
            }

            // Calculating collisions to decide whether the player is allowed to jump
            Player::get_instance().set_is_on_ground(Level::is_colliding({Player::get_instance().get_x(), Player::get_instance().get_y() + 0.1f}, WALL));
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::get_instance().get_is_on_ground()) {
                Player::get_instance().set_y_velocity(-JUMP_STRENGTH);
            }

            Player::get_instance().update();
            Enemy_manager::get_instance().update_all();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED_STATE;
            }
            break;

        case PAUSED_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = GAME_STATE;
            }

            if (IsKeyPressed(KEY_Q)) {
                exit(0);
            }

            break;

        case DEATH_STATE:
            Player::get_instance().update_gravity();

            if (IsKeyPressed(KEY_ENTER)) {
                if (Player::get_instance().get_lives() > 0) {
                    Level::load(0);
                    game_state = GAME_STATE;
                }
                else {
                    game_state = GAME_OVER_STATE;
                    PlaySound(game_over_sound);
                }
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                Level::reset_index();
                Player::get_instance().reset_stats();
                game_state = GAME_STATE;
                Level::load(0);
            }
            break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                Level::reset_index();
                Player::get_instance().reset_stats();
                game_state = MENU_STATE;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }


}

void draw_game() {
    switch(game_state) {
        case MENU_STATE:
            ClearBackground(BLACK);
            draw_menu();
            break;

        case GAME_STATE:
            ClearBackground(BLACK);
            draw_parallax_background();
            draw_level();
            draw_game_overlay();
            break;

        case DEATH_STATE:
            ClearBackground(BLACK);
            draw_death_screen();
            break;

        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            draw_game_over_menu();
            break;

        case PAUSED_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;

        case VICTORY_STATE:
            draw_victory_menu();
            break;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);
    HideCursor();

    load_fonts();
    load_images();
    load_sounds();
    Level::load(0);

    while (!WindowShouldClose()) {
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }

    Level::unload();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
