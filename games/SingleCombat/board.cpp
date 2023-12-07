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


double Board::findTopBlock(int column)
{
        double topmostCoord = boardBottomEdge;
        for(int y = 0; y < grid[column].size(); y++)
        {
            //TODO: are column and y the right way around here?
            if (grid[column][y] != nullptr)
            {
            topmostCoord = boardBottomEdge - (72 * (y + 1));
            }
            
        }
        return topmostCoord;
}


//takes Xcoord given to it. Finds which column in the grid this corresponds to.
int Board::findColumn(double Xcoord)
{
    //note that there is a double involved in this equation. Int returned should be 0, 1, 2, 3, 4 or 5.
    int x = (Xcoord - boardLeftEdge) / 48;
    return x;
}


//this function will get to get/update the destination of the block which will change with key strokes
double Board::getDestination()
{
    double destination = boardBottomEdge;
    double Ycoord = currentBlock->currentY();
    double Xcoord = currentBlock->currentX();

    //std::shared_ptr<Block> blockBelow;

    //steps to this function
    //1. check if grid has anything in it. If it is, just return destination, if not then continue.
    //2. find which column matches the Xcoord of the current block
    //3. In that column, find the first pointer in the array that isn't null
    //4. For the block that pointer is pointing too, get the currentY. destination will be the currentY
    return destination;
    
}





void Board::update()
{

    write_line(findTopBlock(5));

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
        
        if (userInput.checkDownKey())
        {
            //to ensure sprite arrives exactly at destination pixel, moving speed should always be a factor of 72 (block height)
            currentBlock->speedUp(8);
        }

        if (userInput.checkLeftKey())
        {
            //will need to check if columns next to it are available to move to based on X coord
            currentBlock->moveLeft(leftEdge);
        }

        //will need to check if columns next to it are available to move to based on X coord
        if (userInput.checkRightKey())
        {
            
            currentBlock->moveRight(rightEdge);
        }
    }
    else{
    grid[5][0] = currentBlock;
    //currentBlock = nullptr; Will need to be used in phase two where there is temporarily no currentBlock

    //the destination shouldn't be boardBottomEdge for this, will need to be calculated since board won't be empty anymore.
    currentBlock = std::make_shared<Block>(boardBottomEdge, 1);
    }
}
    
