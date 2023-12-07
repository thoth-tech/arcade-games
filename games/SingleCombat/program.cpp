//compile by running skm clang++ *.cpp -o SingleCombat

#include "splashkit.h"
#include "block.h"
#include "board.h"


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
    //Block newBlock;
    Board board;
    

    //window_toggle_border("Single Combat"); uncomment this when ready for arcade
    load_resources();

    
            
    while (!key_typed(ESCAPE_KEY) && !quit_requested())
    {
        process_events();
        clear_screen(COLOR_WHITE);
        //TO DO: fill rectangle draws the 'board' background, this is temporary but in future board should be a bitmap and belong to the board/be updated there.
        //fill rectangle must come first as then board/blocks are drawn and updated after/on-top of it
        fill_rectangle(COLOR_BLACK, 144, 18, 288, 864);
        board.update();
        refresh_screen(60);

    }
    return 0;
}