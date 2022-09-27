#include "Tetromino.h"
#include "Engine.h"

// format {row, col}

int d_tetromino_o[4][4][2] = {
	// 0-degree rotation
	{
		{0, 0},
		{-1, 0},
		{0, 1},
		{-1, 1}
	},
	// 90 degree rotation
	{
		{0, 0},
		{-1, 0},
		{0, 1},
		{-1, 1}
	},
	// 180 degree rotation
	{
		{0, 0},
		{-1, 0},
		{0, 1},
		{-1, 1}
	},
	// 270 degree rotation
	{
		{0, 0},
		{-1, 0},
		{0, 1},
		{-1, 1}
	}
};

int d_tetromino_i[4][4][2] = {
	// 0-degree rotation
	{
		{0, 0},
		{0, -1},
		{0, -2},
		{0, 1}
	},
	// 90 degree rotation
	{
		{0, 0},
		{1, 0}, 
		{-1, 0},
		{-2, 0}
	},
	// 180 degree rotation
	{
		{0, 0},
		{0, -1},
		{0, -2},
		{0, 1}
	},
	// 270 degree rotation
	{
		{0, 0},
		{1, 0}, 
		{-1, 0},
		{-2, 0}
	}
};

int d_tetromino_t[4][4][2] = {
	// 0-degree rotation
	{
		{0, 0},
		{-1, 0},
		{-1, -1},
		{-1, 1}
	},
	// 90 degree rotation
	{
		{0, 0},
		{0, 1}, 
		{-1, 1},
		{1, 1}
	},
	// 180 degree rotation
	{
		{0, 0},
		{1, 0}, 
		{1, 1},
		{1, -1}
	},
	// 270 degree rotation
	{
		{0, 0},
		{0, -1},
		{-1, -1},
		{1, -1}
	},
};

int d_tetromino_j[4][4][2] = {
	{
		{0, 0},
		{1, 0},
		{0, -1},
		{0, -2}
	},
	{
		{0, 0},
		{0, -1},
		{-1, 0},
		{-2, 0}
	},
	{
		{0, 0},
		{-1, 0},
		{0, 1},
		{0, 2}
	},
	{
		{0, 0},
		{0, 1},
		{1, 0},
		{2, 0}
	},
};

int d_tetromino_l[4][4][2] = {
	{
		{0, 0},
		{1, 0},
		{0, 1},
		{0, 2}
	},
	{
		{0, 0},
		{0, -1},
		{1, 0},
		{2, 0}
	},
	{
		{0, 0},
		{-1, 0},
		{0, -1},
		{0, -2}
	},
	{
		{0, 0},
		{0, 1},
		{-1, 0},
		{-2, 0}
	}
};

int d_tetromino_s[4][4][2] = {
	// 0-degree rotation
	{
		{0, 0},
		{0, -1}, 
		{-1, 0},
		{-1, 1}
	},
	// 90 degree rotation
	{
		{0, 0},
		{1, 0},
		{0, -1},
		{-1, -1}
	},
	// 180 degree rotation
	{
		{0, 0},
		{0, -1}, 
		{-1, 0},
		{-1, 1}
	},
	// 270 degree rotation
	{
		{0, 0},
		{1, 0},
		{0, -1},
		{-1, -1}
	}
};

int d_tetromino_z[4][4][2] = {
	// 0-degree rotation
	{
		{0, 0},
		{0, -1},
		{1, 0}, 
		{1, 1}
	},
	// 90 degree rotation
	{
		{0, 0},
		{0, 1},
		{-1, 1},
		{1, 0}
	},
	// 180 degree rotation
	{
		{0, 0},
		{0, -1},
		{1, 0}, 
		{1, 1}
	},
	// 270 degree rotation
	{
		{0, 0},
		{0, 1},
		{-1, 1},
		{1, 0}
	}
};

struct s_tetromino_row_col tetromino_get_row_col_stateless(int current_row, int current_col, int rotation, int i, enum e_tetromino type, int block_size) {
	struct s_tetromino_row_col result = {
		.row = -1,
		.col = -1
	};

	switch (type) {
		case TETROMINO_O:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_o, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_o, i, block_size);
		break;
		case TETROMINO_I:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_i, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_i, i, block_size);
		break;
		case TETROMINO_T:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_t, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_t, i, block_size);
		break;
		case TETROMINO_J:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_j, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_j, i, block_size);
		break;
		case TETROMINO_L:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_l, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_l, i, block_size);
		break;
		case TETROMINO_S:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_s, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_s, i, block_size);
		break;
		case TETROMINO_Z:
			result.row = sk_tetris_tetromino_get_relative_row_stateless(current_row, rotation, d_tetromino_z, i, block_size);
			result.col = sk_tetris_tetromino_get_relative_col_stateless(current_col, rotation, d_tetromino_z, i, block_size);
		break;
		case TETROMINO_UNASSIGNED:
		break;
	}

	return result;
}

struct s_tetromino_row_col tetromino_get_row_col(struct s_tetris_engine *game_engine, int i, enum e_tetromino type) {
	struct s_tetromino_row_col result = {
		.row = -1,
		.col = -1
	};

	switch (type) {
		case TETROMINO_O:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_o, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_o, i);
		break;
		case TETROMINO_I:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_i, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_i, i);
		break;
		case TETROMINO_T:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_t, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_t, i);
		break;
		case TETROMINO_J:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_j, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_j, i);
		break;
		case TETROMINO_L:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_l, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_l, i);
		break;
		case TETROMINO_S:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_s, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_s, i);
		break;
		case TETROMINO_Z:
			result.row = sk_tetris_tetromino_get_relative_row(game_engine->current_tetromino, d_tetromino_z, i);
			result.col = sk_tetris_tetromino_get_relative_col(game_engine->current_tetromino, d_tetromino_z, i);
		break;
		case TETROMINO_UNASSIGNED:
		break;
	}

	return result;
}

struct s_tetromino_color color_combination_one = {
	.first_color = {64, 64, 255},
	.second_color = {255, 255, 255},
	.big_fill = true
};

struct s_tetromino_color color_combination_two = {
	.first_color = {64, 64, 255},
	.second_color = {255, 255, 255},
	.big_fill = false
};

struct s_tetromino_color color_combination_three = {
	.first_color = {72, 162, 255},
	.second_color = {255, 255, 255},
	.big_fill = false
};

struct s_tetromino_color *tetromino_get_color(enum e_tetromino tetromino) {
	switch (tetromino) {
		case TETROMINO_O:
		case TETROMINO_I:
		case TETROMINO_T:
		case TETROMINO_UNASSIGNED:
			return &color_combination_one;

		case TETROMINO_Z:
		case TETROMINO_L:
			return &color_combination_three;

		case TETROMINO_J:
		case TETROMINO_S:
			return &color_combination_two;

	}

	return &color_combination_one;
}