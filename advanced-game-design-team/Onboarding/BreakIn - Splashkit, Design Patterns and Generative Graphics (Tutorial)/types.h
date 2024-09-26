#pragma once

#include <memory>
#include <vector>
#include "../../../games/splashkit mario/include/splashkit/splashkit.h"

struct ivec2;
struct GameState;
struct Paddle;
struct Ball;
struct Block;

/**
 * @brief A 2D integer vector.
 *
 * @details An ivec2 is used to represent a 2D vector with integer components.
 */
struct ivec2 {
    int x, y;
};

/**
 * @brief A Row is a vector of unique pointers to blocks.
 *
 * @details A Row is used to represent a row of blocks in the game.
 */
using Row = std::vector<std::unique_ptr<Block>>;

/**
 * @brief A Grid is a vector of rows.
 *
 * @details A Grid is used to represent the grid of blocks in the game.
 */
using Grid = std::vector<Row>;

/**
 * @brief a Ball is a struct that is used to represent a ball in the game.
 *
 * @details A Ball has a position, velocity, size, color and active state.
 * The active state is used to determine if the ball should be drawn or not.
 */
struct Ball {
    bool active;
    point_2d pos;
    vector_2d vel;
    int size;
    color clr; // `color` is a Splashkit type
};

/**
 * @brief a Paddle is a struct that is used to represent the paddle in the game.
 *
 * @details A Paddle has a position, width, height, and color.
 */
struct Paddle {
    point_2d pos;
    int width;
    int height;
    color clr;
};

/**
 * @brief A block is a small struct that is used to represent the blocks in the game.
 *
 * @details A block has a position, target position, grid position, width, height, color, activity status, and vertical velocity.
 * The target position is used to determine where the block should move to.
 * The target position is just used when the block is due to fall to a new grid position instead of updating instantly.
 * The grid position is used to determine where the block is in the grid (mappable to target pos).
 * The vertical velocity is used to determine how fast the block should move (set to 0 when pos == target_pos).
 */
struct Block {
    bool active;
    point_2d pos;
    point_2d target_pos;
    ivec2 grid_pos;
    int width;
    int height;
    color clr;
    float y_vel;
};

/**
 * @brief A particle is a small struct that is used to represent debris in the game.
 *
 * @details A particle has a position, velocity, size, color, time to live, and original size.
 * The time to live is the number of updates the particle will be drawn for before being removed.
 * The original size is used to determine how the particle will shrink over time.
 * The maximum time to live is used to determine how much of the ttl has lapsed to inform other routines (alpha channel etc)
 */
struct Particle {
    point_2d pos;
    vector_2d vel;
    int size;
    color clr;
    int ttl;
    int max_ttl;
    int original_size;
};

enum GameStatus {
    MENU,
    PLAYING,
    PAUSED,
    ENDED
};

/**
 * @brief A game state is a small struct that is used to represent the state of the game.
 *
 * @details A game state has a game status, score, terrain, balls, particles, and paddle.
 * The game status is used to determine what state the game is in.
 * The score is used to determine the player's score.
 * The terrain is a grid of blocks that is used to represent the blocks in the game.
 * The balls is a vector of balls that is used to represent the balls in the game.
 * The particles is a vector of particles that is used to represent the particles in the game.
 * The paddle is used to represent the paddle in the game.
 */
struct GameState {
    int score;
    GameStatus game_status;
    Paddle paddle;
    std::vector<Ball> balls;
    Grid terrain;
    std::vector<Particle> particles;
};
