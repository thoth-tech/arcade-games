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

            win = update_game(game, levelnum, currentlives);

            currentlives = check_lives(game);

            //Don't think this is functional?
            if (game.player.attacked == true)
                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH / 2 - 138, SCREEN_HEIGHT / 2 - 48, option_to_screen());

            //intended for use when soft-locked/trapped by boxes without needing to restart game. will reset gems and player on current level, but won't reset lives
            if (key_down(R_KEY))
            {
                game = new_game(map);
            }

            //start_debug(game);

            refresh_screen(60);

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
