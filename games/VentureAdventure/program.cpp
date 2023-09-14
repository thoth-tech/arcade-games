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

    string map = "level1.txt";
    int level = 1;
    bool win;
    string levelnum = "Level 1";
    
    game_data game;
    game = new_game(map);

    while(!quit_requested() && !key_down(ESCAPE_KEY)) 
    {
        play_music("intro");
        set_music_volume(0.04);

        start_screen();
    
        fade_music_out(1000);

        play_music("game", 100);
        set_music_volume(0.025);

        while(!quit_requested() && !key_down(ESCAPE_KEY))
        {
            process_events();

            clear_screen();

            draw_game(game);

            win = update_game(game, levelnum);

            if(game.player.attacked == true)
                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH/2-138, SCREEN_HEIGHT/2-48, option_to_screen());

            start_debug(game);

            refresh_screen(60);

            if (win == true && level < 3 )
            {
                level++;
                map = "level";
                map.append(to_string(level));
                map.append(".txt");
                levelnum = "Level ";
                levelnum.append(to_string(level));
                game = new_game(map);
            };

            if (win == true && level == 3)
            {
                delay(5000);
                
                play_music("intro");
                set_music_volume(0.04);

                start_screen();
    
                fade_music_out(1000);

                play_music("game", 100);
                set_music_volume(0.025);

                level = 1;
                map = "level";
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
