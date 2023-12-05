#pragma once
#include "splashkit.h"


class Block
{
public:
    Block();
    void update();
private:
    sprite blockSprite;
    bool moving;
    double destinationY;
    const double boardLeftEdge{144};
    const double boardRightEdge{432};
    double movingSpeed;
    double blockHeight;
    double blockWidth;
};