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
Start: ENTER 
Escape: ESC
*/


void load_resources()
{
    
    load_resource_bundle("game_bundle", "game_bundle.txt");
}


int main()
{
    //max window size 1600 x 900 for Arcade Machine. Minimum 640 x 480
    /*window size of 576 x 864 calibrated to allow for board of 288w x 864h dimensions + some extra space. Dimensions based on blocks that are 75%
    of the original size, so are 48w x 72h. This board can thus fit 12 vertically and 6 horizontally. */
    open_window("Single Combat", 576, 900);

    //window_toggle_border("Single Combat"); uncomment this when ready for arcade
    load_resources();




    while (!key_typed(ESCAPE_KEY) && !quit_requested())
    {
        process_events();
        refresh_screen(60);

        

        //At 75% size (48w x 72h), can easily fit 12 blocks vertically, and plenty of space for 6 horizontally for 2 player screens. Could get away with a bit larger if needed down the track
        //Scaling down for the current art distorts slightly, this will be fine for testing purposes but worth fixing in future (need art with less pixels)
        //Note that the scale down means that the block start is not actually 0, 0. It's offset by 12.5%, so -8 and -12). Will be better to adjust art so that there's no need to scale
        
        //press enter/return to start game
        if (key_typed(RETURN_KEY))
        {
            //game board/play area
            fill_rectangle(COLOR_BLACK, 144, 18, 288, 864);

            //create sprite, set start location and size, should spawn in 3rd column across
            sprite GreenBlockSprite = create_sprite("greenblock.png");
            sprite_set_x(GreenBlockSprite, 232);
            sprite_set_y(GreenBlockSprite, 6);
            sprite_set_scale(GreenBlockSprite, 0.75);
            draw_sprite(GreenBlockSprite);

        }




    }
    return 0;
}