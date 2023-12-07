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
    double currentX();
    void speedUp(double speed);
    void moveLeft(double edge);
    void moveRight(double edge);
    bool checkIfMoving();
private:
    sprite blockSprite;
    colour blockColour;
    bool moving;
    double destinationY;
    double movingSpeed;
    const double blockHeight{72};
    const double blockWidth{48};

   
};