#pragma once
#include <vector>
#include <memory>
#include "block.h"


class Board
{
public:
    Board();
    void update();
private:
    //vector of vectors. Blocks is a vector of columns (a row of columns);
    typedef std::vector<std::shared_ptr<Block>> column;
    std::vector<column> blocks;

    std::shared_ptr<Block> greenBlock;
};