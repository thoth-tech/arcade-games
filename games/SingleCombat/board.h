#pragma once
#include <vector>
#include "block.h"

class Board
{
public:
    Board();
private:
    typedef std::vector<int> rows;
    std::vector<rows> blocks;
};