#include "globals.h"
#include "state_init.h"

// new_paddle takes no arguments as all its properties
// are derived from global constant values
Paddle new_paddle() {
    Paddle paddle;
    // make paddle's initial position center-bottom
    paddle.pos.x = WINDOW_WIDTH / 2;
    paddle.pos.y = WINDOW_HEIGHT - 50;
    paddle.width = INITIAL_PADDLE_WIDTH;
    paddle.height = WINDOW_HEIGHT / 80;
    paddle.clr = clr_paddle;
    return paddle;
}

Ball new_ball(point_2d pos, vector_2d vel, int size, color clr) {
    Ball ball;
    ball.pos = pos;
    ball.vel = vel;
    ball.size = size;
    ball.clr = clr;
    ball.active = true;
    return ball;
}

Block new_block(point_2d pos, point_2d target_pos, ivec2 grid_pos, int width, int height, color c) {
    Block block;
    block.pos = pos;
    block.target_pos = target_pos;
    block.grid_pos = grid_pos;
    block.width = width;
    block.height = height;
    block.clr = c;
    block.active = true;
    block.y_vel = 0.0;
    return block;
}

Grid new_grid() {
    Grid grid;
    for (int y = 0; y < NUM_ROWS; ++y) {
        Row row;
        for (int x = 0; x < NUM_COLS; ++x) {
            point_2d pos = {static_cast<double>(TERRAIN_OFFSET + x * BLOCK_WIDTH), static_cast<double>(y * BLOCK_HEIGHT)};
            ivec2 grid_pos = {x, y};
            row.push_back(std::make_unique<Block>(new_block(pos, pos, grid_pos, BLOCK_WIDTH, BLOCK_HEIGHT, clr_block)));
        }
        grid.push_back(std::move(row));
    }
    return grid;
}

Particle new_particle(point_2d pos, vector_2d vel, color clr, int size, int ttl) {
    Particle particle;
    particle.pos = pos;
    particle.vel = vel;
    particle.size = size;
    particle.clr = clr;
    particle.ttl = ttl;
    particle.max_ttl = ttl;
    particle.original_size = size;
    return particle;
}

GameState new_game_state() {
    GameState game;
    game.score = 0;
    game.game_status = PLAYING;
    game.terrain = new_grid();
    game.paddle = new_paddle();
    game.balls = {};
    return game;
}
