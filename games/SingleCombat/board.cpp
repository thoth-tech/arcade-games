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

    //first block creation, with a pointer to it (currentBlock is the pointer). Not in the grid to start.
    //at the start, the only relevant edges are the left and right board edges, but as more blocks are placed, leftEdge and rightEdge might be edges of other blocks.
    currentBlock = std::make_shared<Block>(boardBottomEdge, 1);
    leftEdge = boardLeftEdge;
    rightEdge = boardRightEdge;


}

void Board::update()
{

    for (int y = 0; y < grid[0].size(); y++)
    {
        for(int x = 0; x < grid.size(); x++)
        {
            if (grid[x][y] != nullptr)
            {
            grid[x][y]->update();
            }
        }
        
    }

    if (currentBlock->checkIfMoving() == true)
    {

        currentBlock->update();
        
        if (userInput.checkDown())
        {
            //to ensure sprite arrives exactly at destination pixel, moving speed should always be a factor of 72 (block height)
            currentBlock->speedUp(8);
        }

        if (userInput.checkLeft())
        {
            
            currentBlock->moveLeft(leftEdge);
        }

        if (userInput.checkRight())
        {
            
            currentBlock->moveRight(rightEdge);
        }
    }
    else{
    grid[0][0] = currentBlock;
    //currentBlock = nullptr; Will need to be used in phase two where there is temporarily no currentBlock

    currentBlock = std::make_shared<Block>(810, 1);
    }
}
    
