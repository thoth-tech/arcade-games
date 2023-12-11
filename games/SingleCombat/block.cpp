#include "block.h"
#include "splashkit.h"

    
    Block::Block(double destination, double speed){
    //create sprite

        //rnd is 0-4 as after some testing, found that rnd did not include the max number as suggested in SplashKit documentation
        blockColour = static_cast<colour>(rnd(0,4));

        switch (blockColour)
        {
            case RED:
            blockSprite = create_sprite("redblock75.png");
            break;
            case BLUE:
            blockSprite = create_sprite("blueblock75.png");
            break;
            case GREEN:
            blockSprite = create_sprite("greenblock75.png");
            break;
            case YELLOW:
            blockSprite = create_sprite("yellowblock75.png");
            break;
        }


        
        //Refer to Game Design folder in Documentation to help calculate block spawn location. Based on a 48w x 72h block as on 5th Dec 2023.
        //Playing board can fit exactly 12 blocks vertically, and 6 blocks horizontally.

        //Sets start location of sprite, should spawn in 3rd column across
        //TODO: The coordinates should be fed to block class from the board class
        sprite_set_x(blockSprite, 240);
        sprite_set_y(blockSprite, 18);

        moving = true;
        movingSpeed = speed;
        destinationY = destination;
        
    }

    //checks if the block is moving, may or may not relate to the Readjustment Phase later on
        bool Block::checkIfMoving()
    {
        return moving;
    }

    //gets the current Y coordinate of the sprite, coordinate is anchored by default to top-left of sprite
    double Block::currentY()
    {
        double currentY = sprite_y(blockSprite);
        return currentY;
    }

 //gets the current X coordinate of the sprite, coordinate is anchored by default to top-left of sprite
    double Block::currentX()
    {
        double currentX = sprite_x(blockSprite);
        return currentX;
    }

    //updates destination (Y coord) of sprite based on what's been given by the board class
    void Block::updateDestination(double destination)
    {
        destinationY = destination;
    }

    //speeds up sprite descent as specified
    void Block::speedUp(double speed)
    {
        movingSpeed = speed;
    }

    //moves sprite left (1 column over)
    //TODO the calulation of coordinate to move to should be done in board class and passed to here as an attribute
    void Block::moveLeft(double edge, double coordinate)
    {
        if (sprite_x(blockSprite) > edge && moving == true)
            {
                sprite_set_x(blockSprite, coordinate);
            }
    }


    //moves sprite left (1 column over)
    //TODO the calulation of coordinate to move to should be done in board class and passed to here as an attribute
    void Block::moveRight(double edge, double coordinate)
    {
        
        if (sprite_x(blockSprite) < edge - blockWidth && moving == true)
            {
                sprite_set_x(blockSprite, coordinate);
            }
    }

    void Block::update(){

        //this loop allows the sprite to move downwards until it reaches it's destination. Splashkit's "sprite move to"function did not allow sprite to stop where it needed to while moving smoothly
        if (moving)
        {
            if (this->currentY() >= destinationY)
            {
                moving = false;
            }
            else{
                //checks that sprite will not accidentally go past the edge of the destination when it gets close. Moving speed would sometimes cause an offset without this.
                if (destinationY < this->currentY() + movingSpeed)
                {
                    sprite_set_y(blockSprite, destinationY);
                }
                else
                {
                    sprite_set_y(blockSprite, this->currentY() + movingSpeed);
                }
            }
            
        }
        update_sprite(blockSprite);
        draw_sprite(blockSprite);
    }
