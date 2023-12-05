#include "board.h"
#include <iostream>

Board::Board(){


    for (int x = 0; x < 6; x++)
    {
        blocks.push_back(std::vector<std::shared_ptr<Block>>());

        for(int y = 0; y < 12; y++)
        {
            blocks[x].push_back(nullptr);
        }
        
    }


}
