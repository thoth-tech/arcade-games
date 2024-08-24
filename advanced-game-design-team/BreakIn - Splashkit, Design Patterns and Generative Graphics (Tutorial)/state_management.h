#pragma once

#include "types.h"

// GLOBAL
/**
 * @brief Update the global state of the game.
 *
 * @param g The game state.
 */
void update_global_state(GameState& g);


// BLOCK
/**
 * @brief Update the block's position.
 *
 * @param b The block to update.
 * @param g The game state.
 */
void block_update(Block& b, GameState& g);

// TERRAIN
/**
 * @brief Update the terrain in the game.
 *
 * @param g The game state.
 */
void update_terrain(GameState& g);

/**
 * @brief Destroy the block.
 *
 * @param b The block to destroy.
 * @param g The game state.
 */
void block_destroy(const Block& b, GameState& g);


//BALL
/**
 * @brief Update the ball's position.
 *
 * @param b The ball to update.
 * @param g The game state.
 */
void ball_update(Ball& b, GameState& g);

///**
// * @brief Destroy the ball.
// *
// * @param b The ball to destroy.
// * @param g The game state.
// */
//void ball_destroy(Ball& b, GameState& g);

/**
 * @brief Check if the ball has collided with the screen edge.
 *
 * @param b The ball to check.
 */
void ball_check_wall_collision(Ball& b);

/**
 * @brief Check if the ball has collided with a block.
 *
 * @param b The ball to check.
 * @param g The game state.
 */
void ball_check_block_collision(Ball& b, GameState& g);

/**
 * @brief Check if the ball has collided with the paddle.
 *
 * @param b The ball to check.
 * @param g The game state.
 */
void ball_check_paddle_collision(Ball& b, GameState& g);

///**
// * @brief Update the particles in the ball's trail.
// *
// * @param b The ball to update the trail for.
// */
//void trail_update(Ball& b);

/**
 * @brief Update the balls in the game.
 *
 * @param g The game state.
 */
void update_balls(GameState& g);



// PADDLE
/**
 * @brief Update the paddle's position.
 *
 * @param g The game state.
 */
void paddle_update(GameState& g);

//
//
// PARTICLE
/**
 * @brief Update the particle's position.
 *
 * @param p The particle to update.
 */
void particle_update(Particle& p);

/**
 * @brief Update the particles in the game.
 *
 * @param g The game state.
 */
void update_particles(GameState& g);

/**
 * @brief Count the number of non-empty rows in the terrain.
 *
 * @param g The game state.
 * @return int The number of non-empty rows.
 */
int count_non_empty_rows(GameState& g);

/**
 * @brief Shift the rows in the terrain down by the given number of rows.
 *
 * @param g The game state.
 * @param num_rows_to_shift The number of rows to shift.
 */
void shift_rows_down(GameState& g, int num_rows_to_shift);

/**
 * @brief Add a new chunk of terrain to the game.
 *
 * @param g The game state.
 * @param num_rows The number of rows in the chunk.
 */
void add_new_chunk(GameState& g, int num_rows);

/**
 * @brief Uses dfs_mark_reachable() to check if blocks are not connected to top row (have been shaved off main body of terrain)
 * and deactivates them.
 *
 * @param g The game state.
 */
void deactivate_disconnected_clusters(GameState& g);

/**
 * @brief Uses DFS to mark blocks that are reachable from the top row.
 *
 * @param g The game state.
 * @param row The row to start the search from.
 * @param col The column to start the search from.
 * @param visited The visited array.
 */
void dfs_mark_reachable(GameState& g, int row, int col, std::vector<std::vector<bool>>& visited);
