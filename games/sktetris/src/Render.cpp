#include "splashkit.h"
#include "Engine.h"
#include "Render.h"
#include "Tetromino.h"

void tetris_render_init(struct s_tetris_engine *game_engine) {
	open_window("Tetris", sk_tetris_translate(SK_TETRIS_WIDTH), sk_tetris_translate(SK_TETRIS_HEIGHT));
}

void tetris_render(struct s_tetris_engine *game_engine) {
	clear_screen(COLOR_BLACK);


	switch (game_engine->game_state) {
	case GAME_STATE_PLAY:
		render_background(game_engine);
		render_board(game_engine);
		render_current_tetromino(game_engine);
		render_next_tetromino(game_engine);
		render_hud(game_engine);

#if SK_TETRIS_RENDER_DEBUG == true
		render_debug(game_engine);
#endif

		break;
	case GAME_STATE_ANIMATE:
		break;
	
	case GAME_STATE_GAMEOVER:
		break;
	}

	refresh_screen();
}

void render_background(struct s_tetris_engine *game_engine) {
	int x = 0 + sk_bitmap_translate(SK_TETRIS_WIDTH, SK_TETRIS_SCALE);
	int y = 0 + sk_bitmap_translate(SK_TETRIS_HEIGHT, SK_TETRIS_SCALE);

	drawing_options opts = option_scale_bmp(
		SK_TETRIS_SCALE, 
		SK_TETRIS_SCALE
	);

	draw_bitmap(game_engine->gfx.frame, x, y, opts);

}

void render_board(struct s_tetris_engine *game_engine) {

	#define BOARD_WIDTH ((SK_TETRIS_ENGINE_COLS) * SK_TETRIS_BLOCK_SIZE)
	#define BOARD_HEIGHT ((SK_TETRIS_ENGINE_ROWS) * SK_TETRIS_BLOCK_SIZE)
	#define BOARD_LEFT ((SK_TETRIS_WIDTH / 2) - (BOARD_WIDTH / 2)) + SK_TETRIS_BLOCK_SIZE -1
	#define BOARD_TOP ((SK_TETRIS_HEIGHT / 2) - (BOARD_HEIGHT / 2)) + SK_TETRIS_BLOCK_SIZE

	fill_rectangle(COLOR_BLACK, sk_tetris_translate(BOARD_LEFT), sk_tetris_translate(BOARD_TOP), sk_tetris_translate(BOARD_WIDTH), sk_tetris_translate(BOARD_HEIGHT));

	for (int r = 0; r < SK_TETRIS_ENGINE_ROWS; ++r) {

		#if SK_TETRIS_DRAW_GRID == true
			// Draw the row line.
			draw_line(
				COLOR_BLUE, 
				line_from(
					sk_tetris_translate(BOARD_LEFT), 
					sk_tetris_translate(BOARD_TOP) + r * sk_tetris_translate(SK_TETRIS_BLOCK_SIZE), 
					sk_tetris_translate(BOARD_LEFT) + sk_tetris_translate(BOARD_WIDTH), 
					sk_tetris_translate(BOARD_TOP) + r * sk_tetris_translate(SK_TETRIS_BLOCK_SIZE)
				)
			);
		#endif

		for (int c = 0; c < SK_TETRIS_ENGINE_COLS; ++c) {

			#if SK_TETRIS_DRAW_GRID == true
				// Draw the col line.
				draw_line(
					COLOR_BLUE, 
					line_from(
						sk_tetris_translate(BOARD_LEFT) + c * sk_tetris_translate(SK_TETRIS_BLOCK_SIZE), 
						sk_tetris_translate(BOARD_TOP), 
						sk_tetris_translate(BOARD_LEFT) + c * sk_tetris_translate(SK_TETRIS_BLOCK_SIZE), 
						sk_tetris_translate(BOARD_TOP + BOARD_HEIGHT)
					)
				);
			#endif

			if (game_engine->board[r][c] != TETROMINO_UNASSIGNED) {
				render_single_block(
					game_engine->board[r][c], 
					sk_tetris_col_get_left(c), 
					sk_tetris_row_get_top(r),
					SK_TETRIS_BLOCK_SIZE
				);
			}

		}
	}

}

void render_current_tetromino(struct s_tetris_engine *game_engine) {

	for (int i=0; i<sizeof(d_tetromino_z) / sizeof(d_tetromino_z[0]); ++i) {
		struct s_tetromino_row_col pos = tetromino_get_row_col(game_engine, i, game_engine->current_tetromino.type);
		if (pos.row < 0 || pos.col < 0 || pos.row >= SK_TETRIS_ENGINE_ROWS || pos.row < 0)
			continue;

		render_single_block(
			game_engine->current_tetromino.type,
			sk_tetris_col_get_left(pos.col),
			sk_tetris_row_get_top(pos.row),
			SK_TETRIS_BLOCK_SIZE
		);
	}

}

void render_stat_tetromino(enum e_tetromino type, int y) {
	int offsetCol = 25;
	int offsetRow = y + 4;

	switch (type) {
		case TETROMINO_J:
			offsetCol += (SK_TETRIS_STAT_BLOCK_SIZE + (SK_TETRIS_STAT_BLOCK_SIZE / 2));
			offsetRow -= (SK_TETRIS_STAT_BLOCK_SIZE);
		break;
		case TETROMINO_L:
			offsetCol -= (SK_TETRIS_STAT_BLOCK_SIZE / 2) + 3;
			offsetRow -= (SK_TETRIS_STAT_BLOCK_SIZE);
		break;
		case TETROMINO_I:
			offsetCol += (SK_TETRIS_STAT_BLOCK_SIZE / 2);
			offsetRow -= (SK_TETRIS_STAT_BLOCK_SIZE / 2) + 1;
		break;
		case TETROMINO_T:
			offsetCol += (SK_TETRIS_STAT_BLOCK_SIZE / 3) + 1;
		break;
		case TETROMINO_S:
			offsetCol += (SK_TETRIS_STAT_BLOCK_SIZE / 4) + 2;
		break;
		case TETROMINO_Z:
			offsetCol += (SK_TETRIS_STAT_BLOCK_SIZE / 4) + 2;
			offsetRow -= (SK_TETRIS_STAT_BLOCK_SIZE) + 1;
		break;
		default:
		break;
	}

	for (int i=0; i<sizeof(d_tetromino_z) / sizeof(d_tetromino_z[0]); ++i) {
		struct s_tetromino_row_col pos = tetromino_get_row_col_stateless(offsetRow, offsetCol, 0, i, type, SK_TETRIS_STAT_BLOCK_SIZE);

		render_single_block(
			type,
			pos.col,
			pos.row,
			SK_TETRIS_STAT_BLOCK_SIZE
		);
	}
}

void render_next_tetromino(struct s_tetris_engine *game_engine) {
	int offsetCol = 200;
	int offsetRow = 128;
	switch (game_engine->next_tetromino) {
		case TETROMINO_J:
			offsetCol += (SK_TETRIS_BLOCK_SIZE + (SK_TETRIS_BLOCK_SIZE / 2)) -1;
			offsetRow -= (SK_TETRIS_BLOCK_SIZE);
		break;
		case TETROMINO_L:
			offsetCol -= (SK_TETRIS_BLOCK_SIZE / 2) + 1;
			offsetRow -= (SK_TETRIS_BLOCK_SIZE);
		break;
		case TETROMINO_I:
			offsetCol += SK_TETRIS_BLOCK_SIZE -1;
			offsetRow -= (SK_TETRIS_BLOCK_SIZE / 2) + 1;
		break;
		case TETROMINO_T:
			offsetCol += (SK_TETRIS_BLOCK_SIZE / 3) + 1;
		break;
		case TETROMINO_S:
			offsetCol += (SK_TETRIS_BLOCK_SIZE / 4) + 1;
		break;
		case TETROMINO_Z:
			offsetCol += (SK_TETRIS_BLOCK_SIZE / 4) + 1;
			offsetRow -= (SK_TETRIS_BLOCK_SIZE) + 1;
		break;
		default:
		break;
	}

	for (int i=0; i<sizeof(d_tetromino_z) / sizeof(d_tetromino_z[0]); ++i) {
		struct s_tetromino_row_col pos = tetromino_get_row_col_stateless(offsetRow, offsetCol, 0, i, game_engine->next_tetromino, SK_TETRIS_BLOCK_SIZE);

		render_single_block(
			game_engine->next_tetromino,
			pos.col,
			pos.row,
			SK_TETRIS_BLOCK_SIZE
		);
	}

}

inline void render_single_block(enum e_tetromino block_type, int left, int top, int block_size) {
	struct s_tetromino_color *color_code = tetromino_get_color(block_type);

	color first_color = rgb_color(color_code->first_color[0], color_code->first_color[1], color_code->first_color[2]);
	color second_color = rgb_color(color_code->second_color[0], color_code->second_color[1], color_code->second_color[2]);

	if (color_code->big_fill) {
		fill_rectangle(COLOR_BLACK, sk_tetris_translate(left), sk_tetris_translate(top), sk_tetris_translate(block_size), sk_tetris_translate(block_size));
		fill_rectangle(first_color, sk_tetris_translate(left + 1), sk_tetris_translate(top + 1), sk_tetris_translate(block_size - 1), sk_tetris_translate(block_size - 1));
		fill_rectangle(second_color, sk_tetris_translate(left + 1), sk_tetris_translate(top + 1), sk_tetris_translate(1), sk_tetris_translate(1));
		fill_rectangle(second_color, sk_tetris_translate(left + 2), sk_tetris_translate(top + 2), sk_tetris_translate(block_size - 3), sk_tetris_translate(block_size - 3));
	} else {
		fill_rectangle(COLOR_BLACK, sk_tetris_translate(left), sk_tetris_translate(top), sk_tetris_translate(block_size), sk_tetris_translate(block_size));
		fill_rectangle(first_color, sk_tetris_translate(left + 1), sk_tetris_translate(top + 1), sk_tetris_translate(block_size - 1), sk_tetris_translate(block_size - 1));
		fill_rectangle(second_color, sk_tetris_translate(left + 1), sk_tetris_translate(top + 1), sk_tetris_translate(1), sk_tetris_translate(1));
		fill_rectangle(second_color, sk_tetris_translate(left + 2), sk_tetris_translate(top + 2), sk_tetris_translate(1), sk_tetris_translate(1));
		fill_rectangle(second_color, sk_tetris_translate(left + 3), sk_tetris_translate(top + 2), sk_tetris_translate(1), sk_tetris_translate(1));
		fill_rectangle(second_color, sk_tetris_translate(left + 2), sk_tetris_translate(top + 3), sk_tetris_translate(1), sk_tetris_translate(1));
	}
}

void render_hud(struct s_tetris_engine *game_engine) {

	// Render lines.
	render_numbers(&game_engine->gfx.numbers, game_engine->line_count, 3, 175, 23);

	// Render level.
	render_numbers(&game_engine->gfx.numbers, game_engine->level, 2, 222, 168);

	// Render top score.
	render_numbers(&game_engine->gfx.numbers, game_engine->top_score, 6, 239, 40);

	// Render score.
	render_numbers(&game_engine->gfx.numbers, game_engine->score, 6, 239, 64);

	// Render stats.
	for (int i=1; i<8; ++i) {
		int y = 78 + (i * 16);
		render_stat_tetromino(static_cast<enum e_tetromino>(i), y);
		render_numbers(&game_engine->gfx.red_numbers, game_engine->tetromino_stats[i], 3, 68, y);
	}

}

void render_single_number(bitmap *number_gfx, int number, int x, int y) {
	if (number < 0 || number > 9)
		return;

	int left = (number * SK_TETRIS_NUMBER_SIZE) + number;

	drawing_options opts = option_part_bmp(left, 0, SK_TETRIS_NUMBER_SIZE, SK_TETRIS_NUMBER_SIZE);
	opts.scale_x = SK_TETRIS_SCALE;
	opts.scale_y = SK_TETRIS_SCALE;

	x = x + (SK_TETRIS_NUMBER_SIZE / 2);
	y = y + (SK_TETRIS_NUMBER_SIZE / 2);

	draw_bitmap(*number_gfx, sk_tetris_translate(x), sk_tetris_translate(y), opts);
}


void render_numbers(bitmap *number_gfx, int number, int digit_length, int right_x, int y) {
	int x = right_x - SK_TETRIS_NUMBER_SIZE;
	int n = number;

	while (digit_length--) {
		int c = n % 10;
		render_single_number(number_gfx, c, x, y);

		n /= 10;
		x -= (SK_TETRIS_NUMBER_SIZE + 1);
	}
}

#if SK_TETRIS_RENDER_DEBUG == true
void render_debug(struct s_tetris_engine *game_engine) {
	std::string str = "Row: " + std::to_string(game_engine->current_tetromino.center_row) + ", Col: " + std::to_string(game_engine->current_tetromino.center_col);
	draw_text(str, COLOR_RED, sk_tetris_translate(10), sk_tetris_translate(10));

	std::string ticker = "Tick: " + std::to_string(game_engine->tick_counter);
	draw_text(ticker, COLOR_RED, sk_tetris_translate(10), sk_tetris_translate(30));

	std::string debounce = "Debounce: " + std::to_string(game_engine->debounce_counter);
	draw_text(debounce, COLOR_RED, sk_tetris_translate(10), sk_tetris_translate(50));

	std::string lines = "Lines: " + std::to_string(game_engine->line_count);
	draw_text(lines, COLOR_WHITE, sk_tetris_translate(10), sk_tetris_translate(70));

	std::string level = "Level: " + std::to_string(game_engine->level);
	draw_text(level, COLOR_WHITE, sk_tetris_translate(10), sk_tetris_translate(90));
}
#endif