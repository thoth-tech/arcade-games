#include "block.h"
#include "splashkit.h"

    
    Block::Block(){
    //create sprite
    blockSprite = create_sprite("greenblock75.png");
        //Refer to /Documentation/singelplayerGameDimensions1.png to help calculate block spawn location. Based on a 48w x 72h block as on 5th Dec 2023.
        //Playing board can fit exactly 12 blocks vertically, and 6 blocks horizontally.

        //Sets start location of sprite, should spawn in 3rd column across
        sprite_set_x(blockSprite, 240);
        sprite_set_y(blockSprite, 18);

        moving = true;
        movingSpeed = 1;

        //TO DO: destination should be a parameter given upon creation.
        //So should colour
        destinationY = 810;
        
    }

    void Block::update(){

        //this loop allows the sprite to move downwards until it reaches it's destination. Splashkit's "sprite move to"function did not allow sprite to stop where it needed to while moving smoothly
        //TO DO: User input needs to be moved out of this class
        if (moving)
        {
            double currentY = sprite_y(blockSprite);
            if (currentY >= destinationY)
            {
                moving = false;
            }
            else{
                //checks that sprite will not go past the edge of the destination based on the moving speed when close
                if (destinationY < currentY + movingSpeed)
                {
                    sprite_set_y(blockSprite, destinationY);
                }
                else
                {
                    sprite_set_y(blockSprite, currentY + movingSpeed);
                }
            }

            if (key_typed(A_KEY))
        {
            if (sprite_x(blockSprite) > boardLeftEdge)
            {
                sprite_set_x(blockSprite, (sprite_x(blockSprite) - blockWidth));
            }

        }

        //Lets block move right if not at edge of board. Offset by -48 due to the sprite anchor point being at it's top left corner (0,0 on bitmap). Sprite is 48px wide
        //TO DO: In future these values, including the size of the board perhaps, should be stored in a variable somewhere, not hard-coded into multiple places. Causes issues if the bitmap size changes in future otherwise.
        if (key_typed(D_KEY))
        {

            if (sprite_x(blockSprite) < boardRightEdge - blockWidth)
            {
                sprite_set_x(blockSprite, (sprite_x(blockSprite) + blockWidth));
            }
            
        }

        if (key_typed(S_KEY))
        {
            //to ensure sprite arrives exactly at destination pixel, moving speed should always be a factor of 72 (block height)
            movingSpeed = 8;
        }

            
        }
        update_sprite(blockSprite);
        draw_sprite(blockSprite);
    }
