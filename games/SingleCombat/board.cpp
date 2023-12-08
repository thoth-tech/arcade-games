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
    leftEdge = boardLeftEdge;
    rightEdge = boardRightEdge;
    activeColumn = 2;
    activeRow = 11;
    changeDestination(activeColumn);
    currentBlock = std::make_shared<Block>(currentDestination, 1);
    

    

}

//this function might not be needed anymore, but checks if entire grid is empty or not
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


//updates activerow and currentdestination for a given column
void Board::changeDestination(int column)
{
        activeRow = 11;
        //currentDestination = boardBottomEdge;
        for(int y = 0; y < grid[column].size(); y++)
        {
            if (grid[column][y] != nullptr)
            {
            //TODO: some sort of bug with this active column
            activeRow = y - 1;
            
            break;
            }   
        }
        currentDestination = boardTopEdge + (72 * activeRow);
        
}


//takes Xcoord given to it. Finds which column in the grid this corresponds to.
void Board::changeActiveColumn(double Xcoord)
{
    //note that there is a double involved in this equation. Int returned should be 0, 1, 2, 3, 4 or 5.
    int x = (Xcoord - boardLeftEdge) / 48;
    activeColumn = x;
}

void Board::update()
{
    //these two lines are just for testing these functions output before grid is fully working
    write_line(activeColumn);
    write_line(activeRow);
    write_line(currentDestination);

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
            changeActiveColumn(currentBlock->currentX());
            changeDestination(activeColumn);
            currentBlock->updateDestination(currentDestination);
        }

        //will need to check if columns next to it are available to move to based on X coord
        if (userInput.checkRightKey())
        {
            currentBlock->moveRight(rightEdge);
            changeActiveColumn(currentBlock->currentX());
            changeDestination(activeColumn);
            currentBlock->updateDestination(currentDestination);
        }
    }
    else{
  
    //need code here to place current block into the correct part of the grid.
    grid[activeColumn][activeRow] = currentBlock;

    activeColumn = 2;
    changeDestination(activeColumn);
    currentBlock = std::make_shared<Block>(currentDestination, 1);
    }
}
    
