/**
 * @file lighting.cpp
 * @brief Implements lighting-related functionality for the game.
 *
 * This file contains functions for updating and calculating lighting effects,
 * including torch lighting and radial light movement.
 */

#include "./include/types.h"
#include "splashkit.h"
#include <cmath>
#include <algorithm>

/**
 * @brief Calculates the breathing effect for the torch radius.
 *
 * @param base_radius The base radius of the torch.
 * @param total_time The total elapsed time.
 * @return The current torch radius after applying the breathing effect.
 */
double calculate_breathing_radius(double base_radius, double total_time) {
    return base_radius + std::sin(total_time * BREATHING_SPEED) * BREATHING_MAGNITUDE;
}

/**
 * @brief Updates the torch position and radius based on player position and time.
 *
 * @param torch The torch to update.
 * @param player The player object.
 * @param total_time The total elapsed time.
 */
void update_torch(Torch& torch, const Player& player, double total_time) {
    torch.position = player.position;
    torch.direction = {cos(player.heading), sin(player.heading)};
    torch.current_radius = calculate_breathing_radius(torch.base_radius, total_time);
}

/**
 * @brief Updates the grid lighting based on radial lights and torch.
 *
 * @param lights The vector of radial lights.
 * @param torch The player's torch.
 * @param torch_on Whether the torch is turned on.
 * @param openclWrapper The OpenCL wrapper for GPU calculations.
 */
void update_grid_lighting(const std::vector<RadialLight>& lights, const Torch& torch, bool torch_on, OpenCLWrapper& openclWrapper) {
    try {
        openclWrapper.calculateLighting(lights, torch, torch_on);
    } catch (const cl::Error& e) {
        write_line("OpenCL error in update_grid_lighting: " + string(e.what()) + " (" + std::to_string(e.err()) + ")");
    } catch (const std::exception& e) {
        write_line("Standard exception in update_grid_lighting: " + string(e.what()));
    } catch (...) {
        write_line("Unknown exception in update_grid_lighting");
    }
}

/**
 * @brief Applies lighting to a base color.
 *
 * @param base_color The original color.
 * @param light_level The light level to apply.
 * @return The resulting color after applying lighting.
 */
color apply_lighting(color base_color, int light_level) {
    double luminosity = AMBIENT_LIGHT + (1.0 - AMBIENT_LIGHT) * (static_cast<double>(light_level) / LIGHT_LEVELS);
    int r = static_cast<int>(luminosity * red_of(base_color));
    int g = static_cast<int>(luminosity * green_of(base_color));
    int b = static_cast<int>(luminosity * blue_of(base_color));
    return rgba_color(r, g, b, 255);
}

/**
 * @brief Updates the position of a single radial light.
 *
 * @param light The radial light to update.
 * @param gridWidth The width of the grid.
 * @param gridHeight The height of the grid.
 * @param deltaTime The time elapsed since the last update.
 */
void update_radial_light_mover(RadialLight& light, int gridWidth, int gridHeight, double deltaTime) {
    const double SPEED = 5.0;

    light.position.x += light.velocity.x * SPEED * deltaTime;
    light.position.y += light.velocity.y * SPEED * deltaTime;

    if (light.position.x < 0 || light.position.x >= gridWidth) {
        light.velocity.x = -light.velocity.x;
        light.position.x = std::max(0.0, std::min(static_cast<double>(gridWidth - 0.01), light.position.x));
    }

    if (light.position.y < 0 || light.position.y >= gridHeight) {
        light.velocity.y = -light.velocity.y;
        light.position.y = std::max(0.0, std::min(static_cast<double>(gridHeight - 0.01), light.position.y));
    }
}

/**
 * @brief Updates the positions of all radial lights.
 *
 * @param lights The vector of radial lights to update.
 * @param gridWidth The width of the grid.
 * @param gridHeight The height of the grid.
 * @param deltaTime The time elapsed since the last update.
 */
void update_radial_light_movers(std::vector<RadialLight>& lights, int gridWidth, int gridHeight, double deltaTime) {
    for (auto& light : lights) {
        update_radial_light_mover(light, gridWidth, gridHeight, deltaTime);
    }
}
