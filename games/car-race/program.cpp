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
bool musicplayed = false;

int main()
{

    open_window("Car Racing", 800, 600);
    load_resources();
    bool game_started = false;
    game_data game = new_game();
    bool onePlayer;
    onePlayer = true;
    while (not quit_requested())
    {
        process_events();
        clear_screen(COLOR_GRAY);
        if (!game_started)
        {
            draw_bitmap("home", 0, 0, option_to_screen());
            refresh_screen(60);
            delay(3000);

            while (!key_down(RETURN_KEY))
            {
                process_events();
                draw_bitmap("select", 0, 0, option_to_screen());

                if (key_down(S_KEY) || key_down(DOWN_KEY))
                {
                    onePlayer = false;
                }
                if (key_down(W_KEY) || key_down(UP_KEY))
                {
                    onePlayer = true;
                }

                if (onePlayer)
                {
                    draw_text("One player", COLOR_RED, "digi", 80, 350, 120, option_to_screen());
                    draw_text("Two players", COLOR_WHITE, "digi", 40, 350, 220, option_to_screen());
                }
                else
                {
                    draw_text("One player", COLOR_WHITE, "digi", 40, 350, 120, option_to_screen());
                    draw_text("Two players", COLOR_RED, "digi", 80, 350, 220, option_to_screen());
                }

                // write_line("onePlayer: " + std::to_string(onePlayer));
                refresh_screen(60);
            }
            game.twoPlayer = !onePlayer;
            game_started = true;
        }
        if (!game.game_over)
        {
            
            if (key_typed(P_KEY))
            {
                game.paused = !game.paused;
            }
            if (game.paused)
            {
                draw_bitmap("paused", 0, 0, option_to_screen());
                refresh_screen(60);
                continue;
            }
            else
            {
                draw_game(game);
                check_collisions(game);
                check_collisions_barricade(game);
                check_collisions_powerup(game);
                out_range(game);
                update_game(game);
            }
            process_events();
        }
        else
        {
            stop_sound_effect("carmotor");
            draw_bitmap("ending", 0, 0, option_to_screen());
            draw_text("Score: " + to_string((int)game.score), COLOR_WHITE, "digi", 70, 100, 400, option_to_screen());
            if (key_typed(RETURN_KEY))
            {
                game = new_game();
                game_started = false;
                stop_sound_effect("endmusic");
            }
            if (!musicplayed)
            {
                play_sound_effect("endmusic", 1);
                musicplayed = true;
            }
        }
        refresh_screen(60);
    }
    return 0;
}
