#pragma once
#include "splashkit.h"


class Block
{
public:
    Block(double destination, double speed);
    void update();
    double currentY();
    void speedUp(double speed);
    void moveLeft();
    void moveRight();
private:
    sprite blockSprite;
    bool moving;
    double destinationY;
    double movingSpeed;
    const double blockHeight{72};
    const double blockWidth{48};
    const double boardLeftEdge{144};
    const double boardRightEdge{432};
};