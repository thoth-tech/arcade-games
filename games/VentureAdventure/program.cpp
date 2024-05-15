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
    open_window("VentureAdventure", 672, SCREEN_HEIGHT);
    window_toggle_border("VentureAdventure");
    load_resources();

    //use this to adjust starting level (for debugging) or to change total number of levels when adding new levels
    string map = "Resources/levels/level1.txt";
    int totallevels = 3;
    int level = 1;
    bool win;
    string levelnum = "Level 1";

    game_data game;
    game = new_game(map);

    bool debugging_output_enabled = true;      // change this to toggle the debugging output on/off
    vector<string> old_debug_message = { "" };
    bool still_waiting = false;

    while (!quit_requested() && !key_down(ESCAPE_KEY))
    {
        play_music("intro");
        set_music_volume(0.04);

        start_screen();

        fade_music_out(1000);

        play_music("game", 100);
        set_music_volume(0.025);

        while (!quit_requested() && !key_down(ESCAPE_KEY))
        {
            process_events();

            clear_screen();

            draw_game(game);

            win = update_game(game, levelnum);

            if (game.player.attacked == true)
                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH / 2 - 138, SCREEN_HEIGHT / 2 - 48, option_to_screen());

            if (debugging_output_enabled)
            {
                // get current debugging info
                vector<string> new_debug_message = get_verbose_debugging_message(game);

                // only send the debug info if the game state has changed since last cycle
                if (old_debug_message != new_debug_message)
                {
                    for(int i = 0; i < new_debug_message.size(); i++)
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

            refresh_screen(60);

            if (win == true && level < totallevels)
            {
                level++;
                map = "Resources/levels/level";
                map.append(to_string(level));
                map.append(".txt");
                levelnum = "Level ";
                levelnum.append(to_string(level));
                game = new_game(map);
            };


            if (win == true && level >= totallevels)
            {
                delay(5000);

                play_music("intro");
                set_music_volume(0.04);

                start_screen();

                fade_music_out(1000);

                play_music("game", 100);
                set_music_volume(0.025);

                level = 1;
                map = "Resources/levels/level";
                map.append(to_string(level));
                map.append(".txt");
                levelnum = "Level ";
                levelnum.append(to_string(level));
                game = new_game(map);
            };
        }
    }

    return 0;
}
