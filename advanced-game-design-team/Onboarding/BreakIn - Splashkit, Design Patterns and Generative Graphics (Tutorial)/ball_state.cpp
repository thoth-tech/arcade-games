#include "state_management.h"
#include "globals.h"

void ball_update(Ball& b, GameState& g) {
    if (!b.active) {
        return;
    }

    // Update ball position
    b.pos.x += b.vel.x;
    b.pos.y += b.vel.y;

    // Check for collisions
    ball_check_wall_collision(b);
    ball_check_block_collision(b, g);
    ball_check_paddle_collision(b, g);
}

void ball_check_wall_collision(Ball& b) {
    if (b.pos.y > GAME_AREA_HEIGHT) {
        b.active = false;
        return;
    }
    if (b.pos.x < GAME_AREA_START + b.size || b.pos.x > GAME_AREA_END - b.size - 1) {
        b.pos.x = (b.pos.x < GAME_AREA_START + b.size) ? GAME_AREA_START + b.size : GAME_AREA_END - b.size - 1;
        b.vel.x *= -1.;
    }
    if (b.pos.y < 0 + b.size) {
        b.pos.y = 0 + b.size;
        b.vel.y *= -1.;
    }
}

void ball_check_block_collision(Ball& b, GameState& g) {
    for (auto& row : g.terrain) {
        for (auto& block : row) {
            if (block && block->active) {
                // Check for collision
                if (b.pos.x < block->pos.x + block->width && b.pos.x + b.size > block->pos.x &&
                    b.pos.y < block->pos.y + block->height && b.pos.y + b.size > block->pos.y) {
                    block->active = false;

                    // Determine collision direction (which side of block was hit)
                    float overlapLeft = (block->pos.x + block->width) - b.pos.x;
                    float overlapRight = (b.pos.x + b.size) - block->pos.x;
                    float overlapTop = (block->pos.y + block->height) - b.pos.y;
                    float overlapBottom = (b.pos.y + b.size) - block->pos.y;

                    bool x_overlap = std::min(overlapLeft, overlapRight) < std::min(overlapTop, overlapBottom);
                    // Reverse velocity based on the smallest overlap
                    if (x_overlap) {
                        b.vel.x *= -1; // Horizontal collision
                    } else {
                        b.vel.y *= -1; // Vertical collision
                    }
                }
            }
        }
    }
}

void ball_check_paddle_collision(Ball& b, GameState& g) {
    if (b.pos.x < g.paddle.pos.x + g.paddle.width && b.pos.x + b.size > g.paddle.pos.x && b.pos.y >= g.paddle.pos.y) {
        b.vel.y *= -1;
        // adjust x velocity based on how close to centre of paddle the ball hits (further to edge = faster x rebound)
        b.vel.x = map_value(b.pos.x - (g.paddle.pos.x + g.paddle.width / 2), 0, g.paddle.width / 2, 0, 5);
    }
}

void update_balls(GameState& g) {
    for (auto& ball : g.balls) {
        ball_update(ball, g);
    }

    // Remove inactive balls from the vector
    g.balls.erase(std::remove_if(g.balls.begin(), g.balls.end(), [](const Ball& b) { return !b.active; }), g.balls.end());
}
