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

    game_data game;
    game = new_game();

    while( not quit_requested() ) 
    {
        play_music("intro");
        set_music_volume(0.04);

        start_screen();
    
        fade_music_out(1000);

        play_music("game", 100);
        set_music_volume(0.025);

        while( not quit_requested() )
        {
            process_events();

            clear_screen();

            draw_game(game);

            update_game(game);

            if(game.player.attacked == true)
                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH/2-138, SCREEN_HEIGHT/2-48, option_to_screen());

            start_debug(game);

            refresh_screen(60);
        }
   }

    return 0;
}