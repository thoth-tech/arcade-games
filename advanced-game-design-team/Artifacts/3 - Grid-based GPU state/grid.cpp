/**
 * @file grid.cpp
 * @brief Implements grid-related functionality for the game.
 *
 * This file contains functions for creating and rendering the game grid,
 * including the generation of obstacles and the application of lighting effects.
 */

#include "./include/types.h"
#include <cmath>
#include <random>
#include <set>

/**
 * @brief Creates a new grid with randomly placed obstacles.
 *
 * @param width The width of the grid.
 * @param height The height of the grid.
 * @return The newly created grid.
 */
Grid create_grid(int width, int height) {
    Grid grid;
    grid.width = width;
    grid.height = height;
    grid.cells.resize(width * height, {HeightLevel::FLOOR, 0, height_to_color(HeightLevel::FLOOR)});

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(0, width - 1);
    std::uniform_int_distribution<> y_dist(0, height - 1);
    std::uniform_int_distribution<> height_dist(0, 2);
    HeightLevel square_height;

    const int num_squares = 100;
    const int square_size = 5;
    std::set<std::pair<int, int>> square_positions;

    for (int i = 0; i < num_squares; ++i) {
        int sx, sy;
        do {
            sx = x_dist(gen);
            sy = y_dist(gen);
        } while (square_positions.count({sx, sy}) > 0);

        square_positions.insert({sx, sy});
        switch(height_dist(gen)) {
            case 0:
                square_height = HeightLevel::BLOCK1;
                break;
            case 1:
                square_height = HeightLevel::BLOCK2;
                break;
            case 2:
                square_height = HeightLevel::BLOCK3;
                break;
            default:
                square_height = HeightLevel::BLOCK1;
        }

        for (int y = sy; y < std::min(sy + square_size, height); ++y) {
            for (int x = sx; x < std::min(sx + square_size, width); ++x) {
                grid.cells[y * width + x].height = square_height;
                grid.cells[y * width + x].base_color = height_to_color(square_height);
            }
        }
    }

    return grid;
}

/**
 * @brief Renders the grid with lighting effects applied.
 *
 * @param openclWrapper The OpenCL wrapper containing grid data.
 */
void render_grid(const OpenCLWrapper& openclWrapper) {
    int gridWidth = openclWrapper.getGridWidth();
    int gridHeight = openclWrapper.getGridHeight();

    std::vector<int> gridHeights;
    std::vector<int> lightLevels;
    openclWrapper.readGridHeights(gridHeights);
    openclWrapper.readLightLevels(lightLevels);

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            int index = y * gridWidth + x;
            HeightLevel height = static_cast<HeightLevel>(gridHeights[index]);
            color base_color = height_to_color(height);
            color final_color = apply_lighting(base_color, lightLevels[index]);
            fill_rectangle(final_color, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
    }
}
