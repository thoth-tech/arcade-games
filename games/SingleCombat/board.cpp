#include "board.h"
#include <iostream>


Board::Board(){

    //creates the grid, initially will all empty pointers
    for (int x = 0; x < 6; x++)
    {
        grid.push_back(blocks());

        for(int y = 0; y < 12; y++)
        {
            grid[x].push_back(nullptr);
        }
        
    }


    leftEdge = boardLeftEdge;
    rightEdge = boardRightEdge;

    //first block creation, with a pointer to it (currentBlock is the pointer). Not in the grid to start. Gives the starting column and destination row (bottom row since grid will be empty)
    activeColumn = 2;
    activeRow = 11;
    //changeDestination here will figure out the destination based on the details above
    changeDestination(activeColumn);
    currentBlock = std::make_shared<Block>(currentDestination, 1);
    

    

}

//updates activerow and currentdestination based on the given column
void Board::changeDestination(int column)
{
        activeRow = 11;
        //currentDestination = boardBottomEdge;
        for(int y = 0; y < grid[column].size(); y++)
        {
            if (grid[column][y] != nullptr)
            {
            activeRow = y - 1;
            break;
            }   
        }
        currentDestination = boardTopEdge + (blockHeight * activeRow);
}

//takes Xcoord given to it. Finds which column in the grid this corresponds to.
void Board::changeActiveColumn(double Xcoord)
{
    //note that there is a double involved in this equation. Int returned should be 0, 1, 2, 3, 4 or 5.
    int x = (Xcoord - boardLeftEdge) / blockWidth;
    activeColumn = x;
}

double Board::calculateMoveLeft()
{
    double newX = currentBlock->currentX() - blockWidth;
    return newX;
}

double Board::calculateMoveRight()
{
    double newX = currentBlock->currentX() + blockWidth;
    return newX;
}





void Board::update()
{

//debug
#ifdef DEBUGSC
    write_line(activeColumn);
    write_line(activeRow);
    write_line(currentDestination);
#endif

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

    //runs while the block is moving (technically 'active' in this case, in later phases there could be moving blocks that are not active/controllable by the player)
    if (currentBlock->checkIfMoving() == true)
    {

        currentBlock->update();
        
        if (userInput.checkDownKey())
        {
            //Speed should ideally always be a factor of 72 (block height) to help block arrive neatly at the destination with little correction
            currentBlock->speedUp(8);
        }

        if (userInput.checkLeftKey())
        {
            //TODO: will need to check if columns next to it are available to move to based on X coord
            currentBlock->moveLeft(leftEdge, calculateMoveLeft());
            changeActiveColumn(currentBlock->currentX());
            changeDestination(activeColumn);
            currentBlock->updateDestination(currentDestination);
        }

        //TODO: will need to check if columns next to it are available to move to based on X coord
        //TODO: duplicate code here as with above, can make into a separte function to update column and destination
        if (userInput.checkRightKey())
        {
            currentBlock->moveRight(rightEdge, calculateMoveRight());
            changeActiveColumn(currentBlock->currentX());
            changeDestination(activeColumn);
            currentBlock->updateDestination(currentDestination);
        }
    }
    else{
  
    //puts the block into the 2D array
    grid[activeColumn][activeRow] = currentBlock;
    
    /*phases may need to have some logic here. There will not always be a currentBlock if the board
    is breaking or readjusting other blocks. Refer to Game Design documentation folder for more information about different phases.
    Essentially don't want player to be moving a new block until the board has done it's thing, something will have to keep track of what phase
    the board is in to determine when it is okay to create a new block
    Part of this would be to make currentBlock = nullptr, temporarily.
    */

    //TODO: can make a function to create a new block (and then use another function to update destination stuff)
    activeColumn = 2;
    changeDestination(activeColumn);
    currentBlock = std::make_shared<Block>(currentDestination, 1);
    }
}
    
