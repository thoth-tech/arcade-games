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
    double movingSpeed;
    const double blockHeight{72};
    const double blockWidth{48};
};