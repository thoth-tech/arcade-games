#pragma once
#include "board.h"


class UserInput
{
public:
    bool checkLeft(){return key_typed(A_KEY);};
    bool checkDown(){return key_typed(S_KEY);};
    bool checkRight(){return key_typed(D_KEY);};
};