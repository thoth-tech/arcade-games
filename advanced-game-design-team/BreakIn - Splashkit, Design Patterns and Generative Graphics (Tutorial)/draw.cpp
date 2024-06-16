#include "draw.h"
#include "globals.h"

void draw_global_state(const GameState& g) {
    clear_screen(clr_background);
    fill_rectangle(color_from_hex("#FBF6E0"), GAME_AREA_START - 3, 0, GAME_AREA_WIDTH + 6, GAME_AREA_HEIGHT - 3);
    fill_rectangle(clr_background, GAME_AREA_START, 0, GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
    draw_text("score: " + std::to_string(g.score), COLOR_WHITE, 20, 20, option_to_screen());
    draw_particles(g);
    draw_terrain(g);
    paddle_draw(g);
    draw_balls(g);
}

void block_draw(const Block& b) {
    if (b.active) {
        fill_rectangle(b.clr, b.pos.x, b.pos.y, b.width, b.height);
    }
}

void draw_terrain(const GameState& g) {
    for (const auto& row : g.terrain) {
        for (const auto& block : row) {
            if (block) {
                block_draw(*block);
            }
        }
    }
}

void ball_draw(const Ball& b) {
    fill_circle(b.clr, b.pos.x, b.pos.y, b.size);
}

void draw_balls(const GameState& g) {
    for (auto& b : g.balls) {
        ball_draw(b);
    }
}

void paddle_draw(const GameState& g) {
    fill_rectangle(g.paddle.clr, g.paddle.pos.x, g.paddle.pos.y, g.paddle.width, g.paddle.height);
}


void particle_draw(const Particle& p) {
    fill_circle(p.clr, p.pos.x, p.pos.y, p.size);
}

void draw_particles(const GameState& g) {
    for (auto& p : g.particles) {
        particle_draw(p);
    }
}
