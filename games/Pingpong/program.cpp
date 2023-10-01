/*
Command:
skm clang++ program.cpp player.cpp game.cpp obstacle.cpp ball.cpp -o kkc
./kkc

*/
#include "splashkit.h"
#include "game.h"
using std::vector;

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "game_bundle.txt");
}
/**
 * Entry point.
 *
 * Manages the initialisation of data, the event loop, and quitting.
 */
int main()
{
    open_window("Something Awesome", 1280, 720);
    load_resources();
    game_data game = new_game();
    while (not quit_requested())
    {
       process_events(); 
       clear_screen(COLOR_AQUA);
       draw_game(game);
       update_game(game);
       refresh_screen(60); 
    }

    return 0;
}
