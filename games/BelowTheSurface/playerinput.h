#include "splashkit.h"
#pragma once

// Updated controles to suit the Arcade Machines

struct player_input
{
    key_code left_key;
    key_code right_key;
    key_code jump_key;
    key_code jump_key2;
    key_code crouch_key;
    key_code attack_key;
};

// Player one input
player_input make_player1_input()
{
    player_input input;
    input.jump_key = UP_KEY;
    input.jump_key2 = RIGHT_ALT_KEY;
    input.right_key = RIGHT_KEY;
    input.left_key = LEFT_KEY;
    input.crouch_key = DOWN_KEY;
    input.attack_key = RIGHT_CTRL_KEY;

    return input;
}

// Player two input
player_input make_player2_input()
{
    player_input input;
    input.jump_key = R_KEY;
    input.jump_key2 = LEFT_ALT_KEY;
    input.right_key = G_KEY;
    input.left_key = D_KEY;
    input.crouch_key = F_KEY;
    input.attack_key = LEFT_CTRL_KEY;

    return input;
}

