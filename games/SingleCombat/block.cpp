#include "splashkit.h"
#include "block.h"
    
    Block::Block(){
    //create sprite
    blockSprite = create_sprite("greenblock75.png");
        //At 100% size (48w x 72h), can easily fit 12 blocks vertically, and plenty of space for 6 horizontally for 2 player screens. Could get away with a bit larger if needed down the track
        //window size currently of 576 x 900 calibrated to allow for board of 288w x 864h dimensions + some extra space
        //set start location of sprite, should spawn in 3rd column across
        sprite_set_x(blockSprite, 232);
        sprite_set_y(blockSprite, 6);
        moving = true;
        destinationY = 882;
        boardLeftEdge = 144;
        boardRightEdge = 288;
    }

    void Block::update(){
        if (moving)
        {
            double currentY = sprite_y(blockSprite);
            if (currentY >= destinationY)
            {
                moving = false;
            }
            else{
                sprite_set_y(blockSprite, currentY + 3);
            }

            if (key_typed(A_KEY))
        {
            if (sprite_x(blockSprite) > boardLeftEdge)
            {
                sprite_set_x(blockSprite, (sprite_x(blockSprite) - 48));
            }

        }

        //lets block move right if not at edge of board
        if (key_typed(D_KEY))
        {

            if (sprite_x(blockSprite) < boardRightEdge + 48)
            {
                sprite_set_x(blockSprite, (sprite_x(blockSprite) + 48));
            }
            
        }
            
        }
        update_sprite(blockSprite);
        draw_sprite(blockSprite);
    }
