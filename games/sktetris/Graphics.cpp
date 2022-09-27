#include "Graphics.h"
#include "splashkit.h"

struct s_tetris_gfx tetris_load_gfx() {
	struct s_tetris_gfx gfx;
	load_bitmap("frame", "frame.png");
	load_bitmap("numbers", "numbers.png");
	load_bitmap("red_numbers", "red_numbers.png");

	gfx.frame = bitmap_named("frame");
	gfx.numbers = bitmap_named("numbers");
	gfx.red_numbers = bitmap_named("red_numbers");

	return gfx;
}