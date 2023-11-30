//compile by running skm clang++ program.cpp -o SingleCombat

#include "splashkit.h"
#include <vector>
using namespace std;

/* CONTROLS INFO
According to https://github.com/thoth-tech/arcade-games
Player One
Up: W | Left: A | Down S | Right: D Weak(1): R | Weak(2): F Med(1) : T | Med(2) : G Hard(1): Y | Hard(2): H

Player Two
Up: UP_ARROW | Left: LEFT_ARROW | Down: DOWN_ARROW | Right: RIGHT_ARROW Weak(1): U | Weak(2): J Med(1) : I | Med(2) : K Hard(1): O | Hard(2): L

General
Start: ENTER Escape: ESC
*/


void load_resources()
{
    
    load_resource_bundle("game_bundle", "game_bundle.txt");
}


int main()
{
    //max window size 1600 x 900 for Arcade Machine. Minimum 640 x 480
    open_window("Single Combat", 1600, 900);
    window_toggle_border("Single Combat");
    load_resources();

    


    while (!key_typed(ESCAPE_KEY) && !quit_requested())
    {
        process_events();
        refresh_screen(60);

        //at 50% size, can easily fit 13 blocks vertically, and plenty of space for 6 horizontally for 2 player screens. Could get away with a bit larger if needed down the track
        //Scaling down for the current art distorts slightly, this will be fine for testing purposes but worth fixing in future (need art with less pixels)
        draw_bitmap("greenblock.png", 1, 50, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("greenbreaker.png", 1, 100, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("redblock.png", 1, 150, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("redbreaker.png", 1, 200, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("blueblock.png", 1, 250, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("bluebreaker.png", 1, 300, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("yellowblock.png", 1, 350, option_scale_bmp(0.50, 0.50, option_to_screen()));
        draw_bitmap("yellowbreaker.png", 1, 400, option_scale_bmp(0.50, 0.50, option_to_screen()));

    }
    return 0;
}