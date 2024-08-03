#include "splashkit.h"
#include "game.h"

using namespace std;

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

    const int TOTAL_LEVELS = 3;     // Remember to update this value when adding new levels
    const float MUSIC_VOLUME_INTRO = 0.04;
    const float MUSIC_VOLUME_LOOP = 0.025;

    string level_map = "Resources/levels/level1.txt";   // Use this to adjust starting level (e.g., for debugging)
    int level_id = 1;
    bool player_won;
    string level_name = "Level 1";

    game_data game;
    game = new_game(level_map);

    bool debugging_output_enabled = true;      // change this to toggle the debugging output on/off
    vector<string> old_debug_message = { "" };
    bool still_waiting = false;

    while (!quit_requested() && !key_down(ESCAPE_KEY))
    {
        play_music("intro");
        set_music_volume(MUSIC_VOLUME_INTRO);

        start_screen();

        fade_music_out(1000);

        play_music("game", 100);
        set_music_volume(MUSIC_VOLUME_LOOP);

        while (!quit_requested() && !key_down(ESCAPE_KEY))
        {
            process_events();

            clear_screen();

            draw_game(game);

            player_won = update_game(game, level_name);

            if (game.player.attacked == true)
            {
                double text_x = SCREEN_WIDTH / 2 - 138;
                double text_y = SCREEN_HEIGHT / 2 - 48;
                int font_size = 70;

                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", font_size, text_x, text_y, option_to_screen());
            }

            if (debugging_output_enabled)
            {
                // get current debugging info
                vector<string> new_debug_message = get_verbose_debugging_message(game);

                // only send the debug info if the game state has changed since last cycle
                if (old_debug_message != new_debug_message)
                {
                    for (int i = 0; i < new_debug_message.size(); i++)
                    {
                        write_line(new_debug_message[i]);
                    }
                    still_waiting = false;
                }
                else if (!still_waiting)
                {
                    write_line("Waiting for game state to change...");
                    still_waiting = true;
                }

                // save the current cycle's message for comparison in the next cycle
                old_debug_message = new_debug_message;
            }

            // intended for use when soft-locked/trapped by boxes without needing to restart game. will reset gems and player on current level, but won't reset lives
            if (key_down(R_KEY))
            {
                game = new_game(level_map);
            }

            refresh_screen(60);

            if (player_won && level_id < TOTAL_LEVELS)
            {
                level_id++;
                level_map = "Resources/levels/level";
                level_map.append(to_string(level_id));
                level_map.append(".txt");
                level_name = "Level ";
                level_name.append(to_string(level_id));
                game = new_game(level_map);
            };

            if (player_won && level_id >= TOTAL_LEVELS)
            {
                delay(5000);

                play_music("intro");
                set_music_volume(MUSIC_VOLUME_INTRO);

                start_screen();

                fade_music_out(1000);

                play_music("game", 100);
                set_music_volume(MUSIC_VOLUME_LOOP);

                level_id = 1;
                level_map = "Resources/levels/level";
                level_map.append(to_string(level_id));
                level_map.append(".txt");
                level_name = "Level ";
                level_name.append(to_string(level_id));
                game = new_game(level_map);
            };
        }
    }

    return 0;
}
