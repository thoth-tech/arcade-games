/**
 * @file player.cpp
 * @brief Implements player-related functionality for the game.
 *
 * This file contains functions for updating and rendering the player,
 * as well as handling player movement and collision detection.
 */

#include "./include/types.h"
#include <cmath>

/**
 * @brief Updates the player's position and state based on input and game rules.
 *
 * @param player The player object to update.
 * @param openclWrapper The OpenCL wrapper for collision detection.
 */
void update_player(Player& player, OpenCLWrapper& openclWrapper) {
    point_2d mouse_pos = mouse_position();
    double dx = mouse_pos.x / CELL_SIZE - player.position.x;
    double dy = mouse_pos.y / CELL_SIZE - player.position.y;
    double target_heading = atan2(dy, dx);

    double angle_diff = target_heading - player.heading;
    if (angle_diff > M_PI) angle_diff -= 2 * M_PI;
    if (angle_diff < -M_PI) angle_diff += 2 * M_PI;

    double turn_amount = PLAYER_TURN_SPEED;
    if (std::abs(angle_diff) < turn_amount) {
        player.heading = target_heading;
    } else {
        player.heading += (angle_diff > 0 ? turn_amount : -turn_amount);
    }

    while (player.heading < 0) player.heading += 2 * M_PI;
    while (player.heading >= 2 * M_PI) player.heading -= 2 * M_PI;

    Vector2D acceleration = {0, 0};
    if (key_down(W_KEY)) acceleration.y -= PLAYER_ACCELERATION;
    if (key_down(S_KEY)) acceleration.y += PLAYER_ACCELERATION;
    if (key_down(A_KEY)) acceleration.x -= PLAYER_ACCELERATION;
    if (key_down(D_KEY)) acceleration.x += PLAYER_ACCELERATION;

    player.velocity.x += acceleration.x;
    player.velocity.y += acceleration.y;

    player.velocity.x *= (1 - PLAYER_FRICTION);
    player.velocity.y *= (1 - PLAYER_FRICTION);

    double speed = sqrt(player.velocity.x * player.velocity.x + player.velocity.y * player.velocity.y);
    if (speed > PLAYER_MAX_SPEED) {
        player.velocity.x = (player.velocity.x / speed) * PLAYER_MAX_SPEED;
        player.velocity.y = (player.velocity.y / speed) * PLAYER_MAX_SPEED;
    }

    double new_x = player.position.x + player.velocity.x;
    double new_y = player.position.y + player.velocity.y;

    int gridWidth = openclWrapper.getGridWidth();
    int gridHeight = openclWrapper.getGridHeight();

    if (new_x >= 0 && new_x < gridWidth && new_y >= 0 && new_y < gridHeight) {
        Vector2D start = player.position;
        Vector2D end = {new_x, new_y};
        Vector2D hit_point;
        openclWrapper.getCollisionPoint(start, end, hit_point);

        if (hit_point.x < 0) {
            player.position.x = new_x;
            player.position.y = new_y;
        } else {
            if (static_cast<int>(new_x) != static_cast<int>(player.position.x)) player.velocity.x = 0;
            if (static_cast<int>(new_y) != static_cast<int>(player.position.y)) player.velocity.y = 0;
        }
    }

    if (player.cooldown > 0) {
        player.cooldown -= 1;
    }
}

/**
 * @brief Renders the player on the screen.
 *
 * @param player The player object to render.
 */
void render_player(const Player& player) {
    double screen_x = player.position.x * CELL_SIZE;
    double screen_y = player.position.y * CELL_SIZE;

    double body_width = CELL_SIZE * 2.5;
    double body_height = CELL_SIZE * 1.5;

    double cos_heading = cos(player.heading);
    double sin_heading = sin(player.heading);

    double body_x1 = screen_x - cos_heading * body_height / 2;
    double body_y1 = screen_y - sin_heading * body_height / 2;
    double body_x2 = screen_x + cos_heading * body_height / 2;
    double body_y2 = screen_y + sin_heading * body_height / 2;

    double perp_x = -sin_heading;
    double perp_y = cos_heading;

    double foot_width = CELL_SIZE * 0.6;
    double foot_height = CELL_SIZE * 0.8;

    double time = player.position.x + player.position.y;
    double animation_speed = 0.6;
    double foot_offset = sin(time * animation_speed) * body_height * 0.8;

    double foot_side_offset = body_width * 0.3;
    double foot1_x = screen_x + perp_x * foot_side_offset + cos_heading * foot_offset;
    double foot1_y = screen_y + perp_y * foot_side_offset + sin_heading * foot_offset;
    double foot2_x = screen_x - perp_x * foot_side_offset - cos_heading * foot_offset;
    double foot2_y = screen_y - perp_y * foot_side_offset - sin_heading * foot_offset;

    auto draw_rotated_rect = [](double cx, double cy, double width, double height, double angle, color clr) {
        double cos_a = cos(angle);
        double sin_a = sin(angle);
        double hwidth = width / 2;
        double hheight = height / 2;

        point_2d p1 = {cx - hwidth * cos_a + hheight * sin_a, cy - hwidth * sin_a - hheight * cos_a};
        point_2d p2 = {cx + hwidth * cos_a + hheight * sin_a, cy + hwidth * sin_a - hheight * cos_a};
        point_2d p3 = {cx + hwidth * cos_a - hheight * sin_a, cy + hwidth * sin_a + hheight * cos_a};
        point_2d p4 = {cx - hwidth * cos_a - hheight * sin_a, cy - hwidth * sin_a + hheight * cos_a};

        fill_triangle(clr, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
        fill_triangle(clr, p1.x, p1.y, p3.x, p3.y, p4.x, p4.y);
    };

    draw_rotated_rect(foot1_x, foot1_y, foot_width, foot_height, player.heading, COLOR_RED);
    draw_rotated_rect(foot2_x, foot2_y, foot_width, foot_height, player.heading, COLOR_RED);

    draw_line(COLOR_BLUE, body_x1, body_y1, body_x2, body_y2, option_line_width(body_width));
    draw_line(COLOR_WHITE, screen_x, screen_y, screen_x + cos_heading * body_height, screen_y + sin_heading * body_height, option_line_width(body_height / 3));

    double line_end_x = screen_x + cos_heading * CELL_SIZE * 2;
    double line_end_y = screen_y + sin_heading * CELL_SIZE * 2;
    draw_line(COLOR_GREEN, screen_x, screen_y, line_end_x, line_end_y);
}

/**
 * @brief Draws a crosshair at the mouse position.
 */
void draw_crosshair() {
    point_2d mouse_pos = mouse_position();
    draw_circle(COLOR_WHITE, mouse_pos.x, mouse_pos.y, 10);
}
