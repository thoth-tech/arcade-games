#pragma once
#include <vector>
#include <memory>
#include "block.h"



class Board
{
public:
    Board();
private:
    typedef std::vector<std::shared_ptr<Block>> rows;
    std::vector<rows> blocks;
};