#pragma once

#include "../../../games/splashkit mario/include/splashkit/splashkit.h"
#include "XOR.h"
#include "util.h"

/**
 * @brief The global window dimensions.
 *
 */
inline constexpr int WINDOW_WIDTH = 1000;
inline constexpr int WINDOW_HEIGHT = 1000;

/**
 * @brief The global game area dimensions.
 *
 */
inline constexpr int GAME_AREA_WIDTH = 600;
inline constexpr int GAME_AREA_HEIGHT = 1000;
inline constexpr int GAME_AREA_OFFSET = (WINDOW_WIDTH - GAME_AREA_WIDTH) / 2;
inline constexpr int GAME_AREA_START = GAME_AREA_OFFSET;
inline constexpr int GAME_AREA_END = GAME_AREA_START + GAME_AREA_WIDTH;

/**
 * @brief The global terrain dimensions and offsets relative to window.
 *
 */
inline constexpr int TERRAIN_OFFSET = GAME_AREA_OFFSET + GAME_AREA_WIDTH / 10;
inline constexpr int TERRAIN_WIDTH = GAME_AREA_WIDTH - GAME_AREA_WIDTH / 5;
inline constexpr int TERRAIN_HEIGHT = TERRAIN_WIDTH;

/**
 * @brief Terrain dimensions
 *
 */
inline constexpr int NUM_ROWS = 50;
inline constexpr int NUM_COLS = 25;

/**
 * @brief The global block dimensions inside the Terrain Grid.
 *
 */
inline constexpr int BLOCK_WIDTH = TERRAIN_WIDTH / NUM_COLS;
inline constexpr int BLOCK_HEIGHT = TERRAIN_HEIGHT / NUM_ROWS;

inline constexpr float BLOCK_POWERUP_CHANCE = 0.02;
inline constexpr int INITIAL_PADDLE_WIDTH = WINDOW_WIDTH / 10;
inline constexpr int MAX_PADDLE_WIDTH = INITIAL_PADDLE_WIDTH * 2;
inline constexpr int MIN_PADDLE_WIDTH = INITIAL_PADDLE_WIDTH / 2;


// rng
/**
 * @brief The global random number generator.
 *
 */
inline XOR rng = XOR();

/**
 * @brief The game palette.
 *
 */
const color clr_background = color_from_hex("#000000");
const color clr_paddle = color_from_hex("#FBF6E0");
const color clr_block = color_from_hex("#FBF6E0");
const color clr_ball_standard = color_from_hex("#FBF6E0");
const color clr_ball_explosion = color_from_hex("#FF2727");
const color clr_ball_acid = color_from_hex("#AFFF26");
