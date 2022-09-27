#include "Engine.h"
#include "Render.h"

int main(void)
{
	struct s_tetris_engine game_engine = tetris_engine_init();

	tetris_render_init(&game_engine);

	// Arcade Machine specific code.
	window_toggle_border("Tetris");

	tetris_loop(&game_engine);

	return 0;
}