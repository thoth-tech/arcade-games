#include "Sound.h"
#include "splashkit.h"

struct s_tetris_sfx tetris_load_sfx() {
	set_resources_path("src/resources");
	load_sound_effect("flip", "flip.ogg");
	load_sound_effect("line", "line.ogg");
	load_sound_effect("set", "set.ogg");
	load_sound_effect("move", "move.ogg");
	load_sound_effect("tetris", "tetris.ogg");
	load_sound_effect("level-up", "level-up.ogg");
	load_music("music-a", "music-a.ogg");

	struct s_tetris_sfx sfx = {
		.flip = sound_effect_named("flip"),
		.line = sound_effect_named("line"),
		.set = sound_effect_named("set"),
		.move = sound_effect_named("move"),
		.tetris = sound_effect_named("tetris"),
		.music_a = music_named("music-a"),
		.level_up = sound_effect_named("level-up")
	};

	return sfx;
}


void tetris_sfx_play(sound_effect *effect) {
	if (sound_effect_playing(*effect))
		stop_sound_effect(*effect);
	play_sound_effect(*effect);
}

void tetris_music_play(music *music) {
	if (music_playing())
		tetris_music_stop();

	play_music(*music);
}

void tetris_music_stop() {
	stop_music();
}


void tetris_loop_music(music *music) {
	if (! music_playing())
		tetris_music_play(music);
}