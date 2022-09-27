#ifndef SK_TETRIS_SOUND_H
#define SK_TETRIS_SOUND_H
#include "splashkit.h"

struct s_tetris_sfx {
	sound_effect flip;
	sound_effect set;
	sound_effect line;
	sound_effect move;
	sound_effect tetris;
	music music_a;
	sound_effect level_up;
};

struct s_tetris_sfx tetris_load_sfx();

void tetris_sfx_play(sound_effect *effect);
void tetris_music_play(music *music);
void tetris_loop_music(music *music);
void tetris_music_stop();

#endif