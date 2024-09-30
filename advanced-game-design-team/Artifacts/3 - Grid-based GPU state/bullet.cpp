/**
 * @file bullet.cpp
 * @brief Implements bullet-related functionality for the game.
 *
 * This file contains functions for creating, updating, and rendering bullets,
 * as well as handling bullet collisions with the environment.
 */

#include "./include/types.h"
#include "splashkit.h"
#include <cmath>

/**
 * @brief Creates a new bullet and adds it to the bullet vector.
 *
 * @param bullets The vector of bullets to add the new bullet to.
 * @param player The player object from which the bullet is fired.
 */
void create_bullet(std::vector<Bullet>& bullets, Player& player) {
    Bullet new_bullet;
    play_sound_effect("gunshot", 1, 0.5);
    new_bullet.position = player.position;
    new_bullet.velocity = {
            cos(player.heading) * BULLET_SPEED,
            sin(player.heading) * BULLET_SPEED
    };
    new_bullet.lifetime = BULLET_LIFETIME;
    player.cooldown = BULLET_COOLDOWN;
    bullets.push_back(new_bullet);
}

/**
 * @brief Updates the state of all bullets and handles collisions.
 *
 * @param bullets The vector of bullets to update.
 * @param particles The vector of particles to add collision effects to.
 * @param openclWrapper The OpenCL wrapper for collision detection.
 */
void update_bullets(std::vector<Bullet>& bullets, std::vector<Particle>& particles, OpenCLWrapper& openclWrapper) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        Vector2D start = it->position;
        Vector2D end = {
                it->position.x + it->velocity.x,
                it->position.y + it->velocity.y
        };

        Vector2D hit_point;
        openclWrapper.getCollisionPoint(start, end, hit_point);
        bool collision = hit_point.x >= 0;

        if (collision) {
            play_sound_effect("hit");
            Vector2D normal;
            int hit_x = static_cast<int>(hit_point.x);
            int hit_y = static_cast<int>(hit_point.y);

            double dx = hit_point.x - start.x;
            double dy = hit_point.y - start.y;

            if (std::abs(dx) > std::abs(dy)) {
                normal.x = (dx > 0) ? -1.0 : 1.0;
                normal.y = 0.0;
            } else {
                normal.x = 0.0;
                normal.y = (dy > 0) ? -1.0 : 1.0;
            }

            create_particles(particles, hit_point, normal, 30);
            openclWrapper.addCollisionPoint(hit_x, hit_y);
            it = bullets.erase(it);
        } else {
            it->position = end;
            it->lifetime--;

            if (it->lifetime <= 0 ||
                it->position.x < 0 || it->position.x >= openclWrapper.getGridWidth() ||
                it->position.y < 0 || it->position.y >= openclWrapper.getGridHeight()) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }
}

/**
 * @brief Renders all bullets on the screen.
 *
 * @param bullets The vector of bullets to render.
 */
void render_bullets(const std::vector<Bullet>& bullets) {
    for (const auto& bullet : bullets) {
        double screen_x = bullet.position.x * CELL_SIZE;
        double screen_y = bullet.position.y * CELL_SIZE;
        drawing_options width = option_line_width(BULLET_RADIUS);

        double alpha = static_cast<double>(bullet.lifetime) / BULLET_LIFETIME;
        color tracer_color = rgba_color(255, 0, 0, static_cast<int>(alpha * 255));
        draw_line(tracer_color,
                  screen_x - bullet.velocity.x * CELL_SIZE * BULLET_TRACER_LENGTH,
                  screen_y - bullet.velocity.y * CELL_SIZE * BULLET_TRACER_LENGTH,
                  screen_x, screen_y, width);
    }
}
