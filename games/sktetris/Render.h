#ifndef SK_TETRIS_RENDER_H
#define SK_TETRIS_RENDER_H
#include "Engine.h"

#define SK_TETRIS_WIDTH 256
#define SK_TETRIS_HEIGHT 240
#define SK_TETRIS_NUMBER_SIZE 7
#define SK_TETRIS_SCALE 3
#define SK_TETRIS_DRAW_GRID false
#define SK_TETRIS_RENDER_DEBUG false

#define sk_tetris_translate(n) ((n) * SK_TETRIS_SCALE)
#define sk_bitmap_translate(s, n) ((s / 2) * (n -1))

#define sk_tetris_col_get_left(c) (BOARD_LEFT + (c) * SK_TETRIS_BLOCK_SIZE)
#define sk_tetris_row_get_top(r) (BOARD_TOP + (r) * SK_TETRIS_BLOCK_SIZE)

void tetris_render_init(struct s_tetris_engine *game_engine);
void tetris_render(struct s_tetris_engine *game_engine);
void render_background(struct s_tetris_engine *game_engine);
void render_board(struct s_tetris_engine *game_engine);
void render_current_tetromino(struct s_tetris_engine *game_engine);
void render_next_tetromino(struct s_tetris_engine *game_engine);
void render_stat_tetromino(enum e_tetromino type, int y);
inline void render_single_block(enum e_tetromino type, int left, int top, int block_size);
void render_hud(struct s_tetris_engine *game_engine);
void render_single_number(bitmap *number_gfx, int number, int x, int y);
void render_numbers(bitmap *number_gfx, int number, int digital_length, int right_x, int y);

#if SK_TETRIS_RENDER_DEBUG == true
void render_debug(struct s_tetris_engine *game_engine);
#endif

#endif