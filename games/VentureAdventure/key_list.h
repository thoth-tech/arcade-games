#pragma once
#include <splashkit.h>
#define P1_B1_KEY       LEFT_CTRL_KEY
#define P1_B2_KEY       LEFT_ALT_KEY
#define P1_START_KEY    NUM_1_KEY

typedef struct key_list
{
    key_code MOVE_UP;
    key_code MOVE_DOWN;
    key_code MOVE_LEFT;
    key_code MOVE_RIGHT;
    key_code RESET_LEVEL;
    key_code START_KEY;
    key_code EXIT_KEY;
} key_list_t;