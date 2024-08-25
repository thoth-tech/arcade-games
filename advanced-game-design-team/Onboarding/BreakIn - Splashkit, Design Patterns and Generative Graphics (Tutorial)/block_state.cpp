#include "state_management.h"
#include "globals.h"
#include "state_init.h"

void block_update(Block& b, GameState& g) {
    if (!b.active) {
        block_destroy(b, g);
        return;
    }

    //debug
    //    if (mouse_x() > b.pos.x && mouse_x() < b.pos.x + b.width && mouse_y() > b.pos.y && mouse_y() < b.pos.y + b.height) {
    //        b.active = false;
    //    }

    // Move the block towards its target position
    if (b.pos.y < b.target_pos.y) {
        b.y_vel += 0.1f; // Increase the vertical velocity
        b.pos.y += b.y_vel;
        if (b.pos.y >= b.target_pos.y) {
            b.pos.y = b.target_pos.y;
            b.y_vel = 0.0f; // Reset the vertical velocity when the block reaches its target position
        }
    }
}

void block_destroy(const Block& b, GameState& g) {
    ++g.score;
    for (int i = 0; i < 2; ++i) {
        vector_2d particle_vel = {rng.randomFloat(-2.0f, 2.0f), rng.randomFloat(2.0f, 0.0f)}; // can't have upward trajectory
        g.particles.push_back(new_particle(b.pos, particle_vel, b.clr, rng.randomInt(1,2), 90));
    }
}
