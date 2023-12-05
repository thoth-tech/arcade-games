#include "splashkit.h"
#include "block.h"
    
    Block::Block(){
    //create sprite
    blockSprite = create_sprite("greenblock.png");
        //At 75% size (48w x 72h), can easily fit 12 blocks vertically, and plenty of space for 6 horizontally for 2 player screens. Could get away with a bit larger if needed down the track
        //Scaling down for the current art distorts slightly, this will be fine for testing purposes but worth fixing in future (need art with less pixels)
        sprite_set_scale(blockSprite, 0.75);
        //set start location of sprite, should spawn in 3rd column across
        //Note that the scale down means that the block start is not actually 0, 0. It's offset by 12.5%, so -8 and -12). Will be better to adjust art so that there's no need to scale
        sprite_set_x(blockSprite, 232);
        sprite_set_y(blockSprite, 6);
        moving = true;
        destinationY = 882;
        boardLeftEdge = 144;
        boardRightEdge = 288;
        //sprite_set_dx(GreenBlockSprite, 1);
        //sprite_set_dy(GreenBlockSprite, 1);
        //sprite_set_heading(GreenBlockSprite, 90);
        //sprite_set_speed(GreenBlockSprite, 1);
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
