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

    //TESTING and DEBUG TOOL. Use these to adjust starting level, to update total levels when adding new level, or adjust lives
    int totallevels = 4;
    int startinglevel = 1;
    int startinglives = 3;


    int currentlevel = startinglevel;
    bool win;
    int currentlives = startinglives;
    string levelnum;
    string map;

    game_data game;
    map = "Resources/levels/level";
    map.append(to_string(currentlevel));
    map.append(".txt");
    levelnum = "Level ";
    levelnum.append(to_string(currentlevel));
    game = new_game(map);

    bool debugging_output_enabled = true;      // change this to toggle the debugging output on/off
    vector<string> old_debug_message = { "" };
    bool still_waiting = false;

    
    // process start screen. could be expanded in future to be a main menu. no way to return to this screen unless game is lost or won, but there's currently no need to go back anyway
    // could also have a proper controls menu/button in future to view controls during the game rather than displayed in the hud (see game.cpp file for hud)
    while (!quit_requested() && !key_down(ESCAPE_KEY))
    {
        play_music("intro");
        set_music_volume(0.04);

        start_screen();

        fade_music_out(1000);

        play_music("game", 100);
        set_music_volume(0.025);

        // processes game/levels
        while (!quit_requested() && !key_down(ESCAPE_KEY))
        {
            process_events();

            clear_screen();

            draw_game(game);

            // updates game. keeps track of win conditions for each level
            win = update_game(game, levelnum, currentlives);

            currentlives = check_lives(game);


            // method 'attack' in game.cpp that changes this variable is not currently function
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

            // intended for use when soft-locked/trapped by boxes without needing to restart game. will reset gems and player on current level, but won't reset lives
            if (key_down(R_KEY))
            {
                game = new_game(map);
            }

            refresh_screen(60);

            // when a level is won, loads next level until the last level
            if (win == true && currentlevel < totallevels)
            {
                currentlevel++;
                map = "Resources/levels/level";
                map.append(to_string(currentlevel));
                map.append(".txt");
                levelnum = "Level ";
                levelnum.append(to_string(currentlevel));
                play_music("game", 100);
                set_music_volume(0.025);
                game = new_game(map);
            }
            // if last level is won or game is lost, returns to main menu. Gamer over sounds and message call are currently within the game.cpp file
            else if ((win == true && currentlevel >= totallevels) || (check_gameover(game) == true))
            {
                if (win)
                {
                    win_screen();
                }
                
                delay(5000);

                play_music("intro");
                set_music_volume(0.04);

                start_screen();

                fade_music_out(1000);

                play_music("game", 100);
                set_music_volume(0.025);

                // resets level conditions
                currentlevel = startinglevel;
                currentlives = startinglives;
                map = "Resources/levels/level";
                map.append(to_string(currentlevel));
                map.append(".txt");
                levelnum = "Level ";
                levelnum.append(to_string(currentlevel));
                game = new_game(map);
            };
        }
    }

    

    return 0;
}
