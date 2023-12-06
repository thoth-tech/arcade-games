#pragma once
#include "splashkit.h"

 enum colour
    {
        RED,
        BLUE,
        GREEN,
        YELLOW
    };

class Block
{
public:
    Block(double destination, double speed);
    void update();
    double currentY();
    void speedUp(double speed);
    void moveLeft();
    void moveRight();
    bool checkIfMoving();
private:
    sprite blockSprite;
    colour blockColour;
    bool moving;
    double destinationY;
    double movingSpeed;
    const double blockHeight{72};
    const double blockWidth{48};
    //TO DO: can these be given to board class? at the moment they are also used in block class.
    const double boardLeftEdge{144};
    const double boardRightEdge{432};

   
};