#include "board.h"
#include <iostream>

Board::Board(){


    for (int x = 0; x < 6; x++)
    {
        grid.push_back(blocks());

        for(int y = 0; y < 12; y++)
        {
            grid[x].push_back(nullptr);
        }
        
    }

    //test creation of a block with a pointer to it (greenBlock is the pointer)
    greenBlock = std::make_shared<Block>();
    grid[3][0] = greenBlock;


}

void Board::update()
{
    greenBlock->update();
}
