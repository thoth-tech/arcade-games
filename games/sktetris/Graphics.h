#ifndef SK_TETRIS_GFX_H
#define SK_TETRIS_GFX_H
#include "splashkit.h"

struct s_tetris_gfx {
	bitmap frame;
	bitmap numbers;
	bitmap red_numbers;
};

struct s_tetris_gfx tetris_load_gfx();

#endif