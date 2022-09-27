#include <cstdlib>
#include <stdbool.h>
#include "Engine.h"
#include "Render.h"
#include "Gameplay.h"
#include "Tetromino.h"
#include "Sound.h"
#include "splashkit.h"

struct s_tetris_engine tetris_engine_init() {
	struct s_tetris_engine game_data;

	// Setup board.
	for (int r=0; r<SK_TETRIS_ENGINE_ROWS; r++) {
		for (int c=0; c<SK_TETRIS_ENGINE_COLS; c++) {
			game_data.board[r][c] = TETROMINO_UNASSIGNED;
		}
	}

	srand(time(NULL));

	game_data.game_state = GAME_STATE_PLAY;
	game_data.line_count = 0;
	game_data.tick_counter = 0;
	game_data.level = 0;
	game_data.score = 0;
	game_data.top_score = 10000;
	game_data.fast_drop = false;
	game_data.debounce_counter = 0;
	game_data.respawn_counter = 0;

	for (int i=0; i<8; ++i)
		game_data.tetromino_stats[i] = 0;


	game_data.current_tetromino.type = get_random_block();
	game_data.tetromino_stats[game_data.current_tetromino.type]++;

	game_data.current_tetromino.center_row = -1;
	game_data.current_tetromino.center_col = SK_TETRIS_ENGINE_COLS / 2 - 1;
	game_data.current_tetromino.rotation = ROTATION_0;

	game_data.next_tetromino = get_random_block();

	game_data.sfx = tetris_load_sfx();
	game_data.gfx = tetris_load_gfx();

	tetris_music_play(&game_data.sfx.music_a);

	return game_data;
}

enum e_tetromino get_random_block() {
	return static_cast<enum e_tetromino>(rand() % (7 - 1 + 1) + 1);
}

void tetris_loop(struct s_tetris_engine *game_engine) {
	while (! quit_requested() && game_engine->game_state != GAME_STATE_GAMEOVER && ! key_down(ESCAPE_KEY)) {
		process_events();

		// Read input.
		tetris_input(game_engine);

		// Process logic.
		tetris_tick(game_engine);

		// Render.
		tetris_render(game_engine);

		// Audio.
		tetris_loop_music(&game_engine->sfx.music_a);

		game_engine->tick_counter++;
		delay(1000 / 60);
	}
}

void tetris_tick(struct s_tetris_engine *game_engine) {

	// If the current tetromino type is unassigned, we
	// are waiting for the respawn counter.
	if (game_engine->current_tetromino.type == TETROMINO_UNASSIGNED) {

		if (++game_engine->respawn_counter >= 18) {
			game_engine->current_tetromino.center_row = -1;
			game_engine->current_tetromino.center_col = (SK_TETRIS_ENGINE_COLS / 2) -1;
			game_engine->current_tetromino.type = game_engine->next_tetromino;
			game_engine->tetromino_stats[game_engine->current_tetromino.type]++;
			game_engine->next_tetromino = get_random_block();
			game_engine->respawn_counter = 0;
		}

		return;
	}

	// Move block down
	if (game_engine->fast_drop) {
		if (game_engine->tick_counter % TETRIS_FAST_DROP_FRAME_SPEED == 0)
			game_engine->current_tetromino.center_row++;
	} else {
		if (game_engine->tick_counter % TETRIS_STATIC_DROP_FRAME_SPEED[game_engine->level] == 0)
			game_engine->current_tetromino.center_row++;
	}

	// When the block is placed
	if (tetromino_collides(game_engine, false)) {
		tetromino_place(game_engine);
		game_engine->current_tetromino.type = TETROMINO_UNASSIGNED;
		game_engine->respawn_counter = 0;
		game_engine->current_tetromino.rotation = ROTATION_0;

		tetris_check_lines(game_engine);
		tetris_check_gameover(game_engine);
	}

}

void tetris_input(struct s_tetris_engine *game_engine) {
	game_engine->fast_drop = key_down(DOWN_KEY) && ! key_down(LEFT_KEY) && ! key_down(RIGHT_KEY);

	// Movement keys are only available when fast drop
	// isn't occurring.
	if (! game_engine->fast_drop) {
		bool debounce = false;
		bool moved = false;

		if (key_down(LEFT_KEY)) {
			debounce = true;

			if (tetris_debounce_should_shift(game_engine->debounce_counter)) {
				game_engine->current_tetromino.center_col--;
				if (tetromino_collides(game_engine, true))
					game_engine->current_tetromino.center_col++;
				else
					moved = true;
			}
		}

		if (key_down(RIGHT_KEY)) {
			debounce = true;
			if (tetris_debounce_should_shift(game_engine->debounce_counter)) {
				game_engine->current_tetromino.center_col++;
				if (tetromino_collides(game_engine, true))
					game_engine->current_tetromino.center_col--;
				else
					moved = true;

			}
		}

		if (moved)
			tetris_sfx_play(&game_engine->sfx.move);

		if (debounce) {
			if (game_engine->debounce_counter == SK_TETRIS_SHIFT_DEBOUNCE_COUNTER)
				game_engine->debounce_counter = SK_TETRIS_SHIFT_DEBOUNCE_REDUCE;
			else
				game_engine->debounce_counter++;
		} else {
			game_engine->debounce_counter = 0;
		}


		if (! game_engine->rotate) {
			if (key_down(SPACE_KEY)) {
				game_engine->rotate = true;
				tetris_rotate(game_engine);

			}
		} else {
			if (! key_down(SPACE_KEY))
				game_engine->rotate = false;
		}
	}

}

void tetris_rotate(struct s_tetris_engine *game_engine) {
	enum e_tetromino_rotation prev = game_engine->current_tetromino.rotation;
	int center_row = game_engine->current_tetromino.center_row;
	int center_col = game_engine->current_tetromino.center_col;

	switch (game_engine->current_tetromino.rotation) {
		case ROTATION_0:
			if (game_engine->current_tetromino.type == TETROMINO_T) {
				game_engine->current_tetromino.center_row--;
				game_engine->current_tetromino.center_col--;
			} else if (game_engine->current_tetromino.type == TETROMINO_L) {
				game_engine->current_tetromino.center_col++;
			} else if (game_engine->current_tetromino.type == TETROMINO_J) {
				game_engine->current_tetromino.center_row++;
			}

			game_engine->current_tetromino.rotation = ROTATION_90;
			break;
		case ROTATION_90:
			if (game_engine->current_tetromino.type == TETROMINO_T) {
				game_engine->current_tetromino.center_col++;
			} else if (game_engine->current_tetromino.type == TETROMINO_L) {
				game_engine->current_tetromino.center_row++;
			} else if (game_engine->current_tetromino.type == TETROMINO_J) {
				game_engine->current_tetromino.center_col--;
			}

			game_engine->current_tetromino.rotation = ROTATION_180;
		break;
		case ROTATION_180:
			if (game_engine->current_tetromino.type == TETROMINO_T) {
				game_engine->current_tetromino.center_row++;
				game_engine->current_tetromino.center_col++;
			} else if (game_engine->current_tetromino.type == TETROMINO_L) {
				game_engine->current_tetromino.center_col--;
			} else if (game_engine->current_tetromino.type == TETROMINO_J) {
				game_engine->current_tetromino.center_row--;
			}

			game_engine->current_tetromino.rotation = ROTATION_270;
		break;
		case ROTATION_270:
			if (game_engine->current_tetromino.type == TETROMINO_T) {
				game_engine->current_tetromino.center_col--;
				game_engine->current_tetromino.center_row++;
			} else if (game_engine->current_tetromino.type == TETROMINO_L) {
				game_engine->current_tetromino.center_row--;
			} else if (game_engine->current_tetromino.type == TETROMINO_J) {
				game_engine->current_tetromino.center_col++;
			}

			game_engine->current_tetromino.rotation = ROTATION_0;
		break;
	}

	if (tetromino_collides(game_engine, true)) {
		game_engine->current_tetromino.rotation = prev;
		game_engine->current_tetromino.center_row = center_row;
		game_engine->current_tetromino.center_col = center_col;

	} else {
		tetris_sfx_play(&game_engine->sfx.flip);
	}
}

bool tetromino_collides(struct s_tetris_engine *game_engine, bool check_edges) {

	for (int i=0; i<sizeof(d_tetromino_z) / sizeof(d_tetromino_z[0]); ++i) {
		struct s_tetromino_row_col pos = tetromino_get_row_col(game_engine, i, game_engine->current_tetromino.type);

		if (check_edges) {
			if (pos.col < 0)
				return true;

			if (pos.col > SK_TETRIS_ENGINE_COLS -1)
				return true;
		}

		if (pos.row < 0 || pos.col < 0)
			continue;

		if (game_engine->board[pos.row][pos.col] != TETROMINO_UNASSIGNED)
			return true;

		if (pos.row == SK_TETRIS_ENGINE_ROWS)
			return true;
	}

	return false;

}

void tetromino_place(struct s_tetris_engine *game_engine) {
	tetris_sfx_play(&game_engine->sfx.set);

	for (int i=0; i<4; ++i) {
		struct s_tetromino_row_col pos = tetromino_get_row_col(game_engine, i, game_engine->current_tetromino.type);

		if (pos.row < 0 || pos.col < 0)
			continue;

		game_engine->board[pos.row -1][pos.col] = game_engine->current_tetromino.type;
	}
}


void tetris_check_lines(struct s_tetris_engine *game_data) {
	int row = SK_TETRIS_ENGINE_ROWS;
	bool lines[SK_TETRIS_ENGINE_ROWS];
	bool has_lines = false;

	while (row--) {
		lines[row] = false;

		int col = SK_TETRIS_ENGINE_COLS;
		bool failed = false;
		while (col--) {
			if (game_data->board[row][col] == TETROMINO_UNASSIGNED) {
				failed = true;
				break;
			}
		}

		if (! failed) {
			lines[row] = true;
			has_lines = true;
		}
	}

	if (has_lines) {
		tetris_clear_lines(game_data, lines);
		tetris_set_level(game_data);
	}

}

void tetris_clear_lines(struct s_tetris_engine *game_data, bool lines[SK_TETRIS_ENGINE_ROWS]) {
	int offset = 0;
	int line_count = 0;
	for (int row=0; row<SK_TETRIS_ENGINE_ROWS; ++row) {
		if (! lines[row])
			continue;

		int remove_row = row + offset;

		// Unassign the row.
		for (int c=0; c<SK_TETRIS_ENGINE_COLS; ++c)
			game_data->board[remove_row][c] = TETROMINO_UNASSIGNED;

		// Shift everything down.
		for (int rr=remove_row -1; rr >= 0; rr--) {
			for (int c=0; c<SK_TETRIS_ENGINE_COLS; ++c)
				game_data->board[rr +1][c] = game_data->board[rr][c];
		}

		line_count++;
	}

	if (line_count > 0)
		tetris_sfx_play(&game_data->sfx.line);

	if (line_count >= 4)
		tetris_sfx_play(&game_data->sfx.tetris);

	game_data->line_count += line_count;
	tetris_increment_score(game_data, line_count);
}

void tetris_increment_score(struct s_tetris_engine *game_data, int line_count) {
	if (line_count < 1)
		return;

	int multiplier = 40;
	if (line_count == 2)
		multiplier = 100;
	else if (line_count == 3)
		multiplier = 300;
	else if (line_count == 4)
		multiplier = 1200;

	game_data->score += (multiplier * (game_data->level + 1));
}

void tetris_set_level(struct s_tetris_engine *game_data) {
	if (game_data->level > 13)
		return;

	// In Marathon (called A-TYPE), when the player line clear (startLevel × 10 + 10) or max(100, (startLevel × 10 - 50)) 
	// lines, whatever comes first, the level advances by 1. After this, the level advances by 1 for every 10 lines.
	if ((game_data->line_count >= (game_data->level * 10) + 10)) {
		game_data->level++;
		tetris_sfx_play(&game_data->sfx.level_up);
	}

}

void tetris_check_gameover(struct s_tetris_engine *game_data) {
	for (int c=0; c<SK_TETRIS_ENGINE_COLS; ++c)  {
		if (game_data->board[0][c] != TETROMINO_UNASSIGNED) {
			game_data->game_state = GAME_STATE_GAMEOVER;
			break;
		}
	}
}