#ifndef SK_TETRIS_GAMEPLAY_H
#define SK_TETRIS_GAMEPLAY_H

#define SK_TETRIS_MAX_LEVEL 29
#define SK_TETRIS_SHIFT_DEBOUNCE_COUNTER 13
#define SK_TETRIS_SHIFT_DEBOUNCE_REDUCE 8

#define tetris_debounce_should_shift(debounce_counter) (debounce_counter == 0 || debounce_counter == SK_TETRIS_SHIFT_DEBOUNCE_COUNTER)

const int TETRIS_STATIC_DROP_FRAME_SPEED[] = {
	48,
	43,
	38, 
	33,
	28,
	23,
	18,
	13,
	8,
	6,
	5,
	4,
	3,
	2,
	1
};
#define TETRIS_FAST_DROP_FRAME_SPEED 1

#endif