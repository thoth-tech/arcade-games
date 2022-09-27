#ifndef SK_TETRIS_TETROMINO_H
#define SK_TETRIS_TETROMINO_H

struct s_tetromino_row_col {
	int row;
	int col;
};

struct s_tetromino_color {
	int first_color[3];
	int second_color[3];
	bool big_fill;
};

enum e_tetromino {
	TETROMINO_UNASSIGNED = 0,
	TETROMINO_T = 1,
	TETROMINO_J = 2,
	TETROMINO_Z = 3,
	TETROMINO_O = 4,
	TETROMINO_S = 5,
	TETROMINO_L = 6,
	TETROMINO_I = 7
};

enum e_tetromino_rotation {
	ROTATION_0 = 0,
	ROTATION_90 = 1,
	ROTATION_180 = 2,
	ROTATION_270 =3
};

extern int d_tetromino_o[4][4][2];
extern int d_tetromino_i[4][4][2];
extern int d_tetromino_t[4][4][2];
extern int d_tetromino_j[4][4][2];
extern int d_tetromino_l[4][4][2];
extern int d_tetromino_s[4][4][2];
extern int d_tetromino_z[4][4][2];

#define sk_tetris_tetromino_get_relative_row(current_tetromino, tetromino_data, index) current_tetromino.center_row + tetromino_data[current_tetromino.rotation][index][0]
#define sk_tetris_tetromino_get_relative_row_stateless(center_row, rotation, tetromino_data, index, block_size) center_row + (tetromino_data[rotation][index][0] * block_size)
#define sk_tetris_tetromino_get_relative_col(current_tetromino, tetromino_data, index) current_tetromino.center_col + tetromino_data[current_tetromino.rotation][index][1]
#define sk_tetris_tetromino_get_relative_col_stateless(center_col, rotation, tetromino_data, index, block_size) center_col + (tetromino_data[rotation][index][1] * block_size)

int ***tetromino_get_shape(enum e_tetromino type);
struct s_tetromino_row_col tetromino_get_row_col(struct s_tetris_engine *game_engine, int i, enum e_tetromino type);
struct s_tetromino_row_col tetromino_get_row_col_stateless(int current_row, int current_col, int rotation, int i, enum e_tetromino type, int block_size);
struct s_tetromino_color *tetromino_get_color(enum e_tetromino tetromino);

#endif