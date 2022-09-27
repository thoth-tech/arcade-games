#ifndef SK_TETRIS_ENGINE_H
#define SK_TETRIS_ENGINE_H
#include <stdbool.h>
#include "Tetromino.h"
#include "Sound.h"
#include "Graphics.h"

#define SK_TETRIS_ENGINE_ROWS 20
#define SK_TETRIS_ENGINE_COLS 10
#define SK_TETRIS_BLOCK_SIZE 8
#define SK_TETRIS_STAT_BLOCK_SIZE 6


enum e_game_state {
	GAME_STATE_PLAY,
	GAME_STATE_ANIMATE,
	GAME_STATE_GAMEOVER
};

struct s_tetromino_location {
	enum e_tetromino type;
	enum e_tetromino_rotation rotation;
	int center_row;
	int center_col;
};

struct s_tetris_engine {
	enum e_tetromino board[SK_TETRIS_ENGINE_ROWS][SK_TETRIS_ENGINE_COLS];
	enum e_game_state game_state;
	struct s_tetromino_location current_tetromino;
	enum e_tetromino next_tetromino;
	int tetromino_stats[8];
	int line_count;
	int level;
	int score;
	int top_score;
	bool fast_drop;
	bool rotate;
	unsigned int tick_counter;
	unsigned int debounce_counter;
	unsigned int respawn_counter;
	struct s_tetris_sfx sfx;
	struct s_tetris_gfx gfx;
};

struct s_tetris_engine tetris_engine_init();

enum e_tetromino get_random_block();
void tetris_loop(struct s_tetris_engine *game_data);
void tetris_tick(struct s_tetris_engine *game_data);
void tetris_rotate(struct s_tetris_engine *game_engine);
void tetris_input(struct s_tetris_engine *game_data);
void tetris_check_lines(struct s_tetris_engine *game_data);
void tetris_clear_lines(struct s_tetris_engine *game_data, bool lines[SK_TETRIS_ENGINE_ROWS]);
void tetris_increment_score(struct s_tetris_engine *game_data, int line_count);
void tetris_set_level(struct s_tetris_engine *game_data);
void tetris_check_gameover(struct s_tetris_engine *game_data);

bool tetromino_collides(struct s_tetris_engine *game_engine, bool check_edges);
void tetromino_place(struct s_tetris_engine *game_engine);


#endif