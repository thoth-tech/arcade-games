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
    double boardLeftEdge;
    double boardRightEdge;
    double movingSpeed;
    double blockHeight;
    double blockWidth;
};