/*
Run this by:
skm clang++ program.cpp player.cpp game.cpp -o output
./output
*/
#include "splashkit.h"
#include "game.h"
using std::vector;
using namespace std;

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "game_bundle.txt");
}
bool musicplayed =false;
int main()
{

    open_window("Car Racing", 800, 600);
    load_resources();
    bool game_started = false;
    game_data game = new_game();
    while (not quit_requested())
    {
        process_events();
        clear_screen(COLOR_GRAY);
        if (!game_started)
        {
            draw_bitmap("home", 0, 0, option_to_screen());
            refresh_screen(60);
            stop_sound_effect("endmusic");
            play_sound_effect("gameplaymusic");
            delay(3000);
            game_started = true;
        }
        if(!game.game_over) {
        
        draw_game(game);
        check_collisions(game);
        out_range(game);
        update_game(game);
        } else {
            stop_sound_effect("carmotor");
            stop_sound_effect("gameplaymusic");
            draw_bitmap("ending", 0, 0, option_to_screen());
            draw_text("Score: "+ to_string((int)game.score), COLOR_WHITE, "digi", 70, 100, 400, option_to_screen());
            if(key_typed(RETURN_KEY)) {
                game = new_game();
                game_started = false;
                
                
                play_sound_effect("gameplaymusic");
                stop_sound_effect("endmusic");
            }
            if(!musicplayed) {
                play_sound_effect("endmusic", 1);
             
                musicplayed = true;
            }
            else
            {
                if (key_typed(RETURN_KEY))
                {
                    stop_sound_effect("endmusic");
                    musicplayed = false;  // Reset musicplayed flag
                }

                
            }
        
        }
        refresh_screen(60);
    }

    return 0;
}
