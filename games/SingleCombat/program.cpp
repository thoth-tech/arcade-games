//compile by running skm clang++ *.cpp -o SingleCombat

#include "splashkit.h"
#include <vector>
#include "block.h"
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
    /*window size of 576 x 900 calibrated to allow for board of 288w x 864h dimensions + some extra space.
    of the original size, so are 48w x 72h. This board can thus fit 12 vertically and 6 horizontally. */
    open_window("Single Combat", 576, 900);
    Block newBlock;
    
    
    //variable to define edge of the playing board.
    


    //window_toggle_border("Single Combat"); uncomment this when ready for arcade
    load_resources();

    
            
    while (!key_typed(ESCAPE_KEY) && !quit_requested())
    {
        process_events();
        clear_screen(COLOR_WHITE);
        fill_rectangle(COLOR_BLACK, 144, 18, 288, 864);
        newBlock.update();
        //moveTo(GreenBlockSprite, 882);
        refresh_screen(60);

        //lets block move left if not at edge of board
        /*if (key_typed(A_KEY))
        {
            if (sprite_x(GreenBlockSprite) > boardLeftEdge)
            {
                sprite_set_x(GreenBlockSprite, (sprite_x(GreenBlockSprite) - 48));
            }

        }

        //lets block move right if not at edge of board
        if (key_typed(D_KEY))
        {

            if (sprite_x(GreenBlockSprite) < boardRightEdge + 48)
            {
                sprite_set_x(GreenBlockSprite, (sprite_x(GreenBlockSprite) + 48));
            }
            
        }

        //speeds descent of block, can't be undone
        if (key_typed(S_KEY))
        {
            sprite_set_speed(GreenBlockSprite, 10);
        }*/

    }
    return 0;
}