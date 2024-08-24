#include "splashkit.h"
#include "game.h"
#include "debugging.h"

void load_resources()
{
    load_resource_bundle("game_bundle", "icons.txt");
    load_music("game", "game.wav");
    load_music("intro", "intro.mp3");
    open_audio();
}

int main()
{
    const string WINDOW_NAME = "VentureAdventure";
    const int WINDOW_WIDTH = 672;
    const int WINDOW_HEIGHT = SCREEN_HEIGHT;

    open_window(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
    window_toggle_border(WINDOW_NAME);
    load_resources();

    const float MUSIC_VOLUME_INTRO = 0.05;
    const float MUSIC_VOLUME_LOOP = 0.03;
    
    const int TOTAL_LEVELS = 4;
    const int STARTING_LEVEL = 1;
    const int STARTING_LIVES = 3;

    int level_id = STARTING_LEVEL;
    int current_lives = STARTING_LIVES;
    string level_map = "Resources/levels/level1.txt";
    bool is_level_completed = false;

    game_data game;
    game = new_game(level_map);

    bool is_debugging_output_enabled = false;      // Change this to toggle the debugging output on/off
    vector<string> old_debug_message = { "" };
    bool still_waiting = false;
    
    // process start screen. could be expanded in future to be a main menu. no way to return to this screen unless game is lost or won, but there's currently no need to go back anyway
    // could also have a proper controls menu/button in future to view controls during the game rather than displayed in the hud (see game.cpp file for hud)
    while (!quit_requested() && !key_down(ESCAPE_KEY))
    {
        play_music("intro");
        set_music_volume(MUSIC_VOLUME_INTRO);

        start_screen();
        fade_music_out(1000);

        // processes game/levels
        while (!quit_requested() && !key_down(ESCAPE_KEY))
        {
            process_events();
            clear_screen();
            draw_game(game);

            is_level_completed = update_game(game, level_id, current_lives);

            if (game.player.attacked == true)
            {
                double text_x = SCREEN_WIDTH / 2 - 138;
                double text_y = SCREEN_HEIGHT / 2 - 48;
                int font_size = 70;

                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", font_size, text_x, text_y, option_to_screen());
            }

            if (is_debugging_output_enabled)
            {
                process_debugging(game, old_debug_message, still_waiting);
            }

            // Intended for use when soft-locked/trapped by boxes without needing to restart game. Will reset gems and player on current level, but won't reset lives
            if (key_down(R_KEY))
            {
                game = new_game(level_map);
            }

            refresh_screen(60);

            // When a level is won, loads next level until the last level
            if (is_level_completed && level_id < TOTAL_LEVELS)
            {
                level_id++;
                level_map = "Resources/levels/level" + std::to_string(level_id) + ".txt";
                play_music("game", 100);
                set_music_volume(0.025);

                write_line(level_map);
                game = new_game(level_map);
            }
            // If last level is won or game is lost, returns to main menu. Gamer over sounds and message call are currently within the game.cpp file
            else if ((is_level_completed && level_id >= TOTAL_LEVELS) || (check_gameover(game)))
            {
                if (is_level_completed)
                {
                    win_screen();
                }
                
                delay(5000);

                play_music("intro");
                set_music_volume(MUSIC_VOLUME_INTRO);

                start_screen();

                fade_music_out(1000);

                play_music("game", 100);
                set_music_volume(MUSIC_VOLUME_LOOP);

                // Resets level conditions
                level_id = STARTING_LEVEL;
                current_lives = STARTING_LIVES;
                level_map = "Resources/levels/level" + std::to_string(level_id) + ".txt";

                write_line(level_map);
                game = new_game(level_map);
            };
        }
    }

    

    return 0;
}
