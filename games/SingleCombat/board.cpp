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

//checks if grid is empty
bool Board::checkIfEmpty(){

    for (int y = 0; y < grid[0].size(); y++)
    {
        for(int x = 0; x < grid.size(); x++)
        {
            if (grid[x][y] != nullptr)
            {
            return false;
            }
        } 
    }
    return true;

}

//this function will get to get/update the destination of the block which will change with key strokes
double Board::getDestination()
{
    double destination = boardBottomEdge;
    double Ycoord = currentBlock->currentY();
    double Xcoord = currentBlock->currentX();
    //steps to this function
    //1. check if grid has anything in it, loop through all but if any are not null, we can continue. if it is, just return destination



    //2. find which column matches the Xcoord of the current block
    //3. In that column, find the first pointer in the array that isn't null
    //4. For the block that pointer is pointing too, get the currentY. destination will be the currentY
    //in theory you could just calculate the destination by the array ID (+1), but this would not be messy if the block dimensions ever changed as would be using an assumed constant
    return destination;
    
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

    //the destination shouldn't be boardBottomEdge for this, will need to be calculated since board won't be empty anymore.
    currentBlock = std::make_shared<Block>(boardBottomEdge, 1);
    }
}
    
