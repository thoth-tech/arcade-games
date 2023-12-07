#pragma once
#include "board.h"


class UserInput
{
public:
    bool checkLeftKey(){return key_typed(A_KEY);};
    bool checkDownKey(){return key_typed(S_KEY);};
    bool checkRightKey(){return key_typed(D_KEY);};
};