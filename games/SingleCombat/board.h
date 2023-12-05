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
    //vector of vectors. A grid of blocks
    typedef std::shared_ptr<Block> pointer;
    typedef std::vector<pointer> blocks;
    std::vector<blocks> grid;

    std::shared_ptr<Block> greenBlock;
};