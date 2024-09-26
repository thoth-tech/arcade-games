#include "./include/types.h"
#include <algorithm>

Grid create_grid() {
    int cells_x = RES;
    int cells_y = RES;
    return {std::vector<Cell>(cells_x * cells_y, {CellType::Floor, 0.0}), cells_x, cells_y};
}

void set_cell(Grid& grid, int x, int y, CellType type) {
    if (x >= 0 && x < grid.width && y >= 0 && y < grid.height) {
        grid.cells[y * grid.width + x].type = type;
    }
}

Cell get_cell(const Grid& grid, int x, int y) {
    if (x >= 0 && x < grid.width && y >= 0 && y < grid.height) {
        return grid.cells[y * grid.width + x];
    }
    return {CellType::Wall, 0.0}; // Return wall for out-of-bounds
}

void march_light(Grid& grid, point_2d light_source, int num_rays, double max_distance) {
    // Reset luminance
    for (auto& cell : grid.cells) {
        cell.luminance = 0.0;
    }

    for (int i = 0; i < num_rays; ++i) {
        double angle = i * (360.0 / num_rays);
        vector_2d direction = vector_from_angle(angle, 1.0);

        // Current position along the ray
        double current_x = light_source.x;
        double current_y = light_source.y;

        double total_distance = 0.0;

        while (total_distance <= max_distance) {
            int cell_x = static_cast<int>(current_x / CELL_SIZE);
            int cell_y = static_cast<int>(current_y / CELL_SIZE);

            // Calculate next cell in both X and Y directions
            int next_cell_x = static_cast<int>((current_x + direction.x * (CELL_SIZE / 2.0)) / CELL_SIZE);
            int next_cell_y = static_cast<int>((current_y + direction.y * (CELL_SIZE / 2.0)) / CELL_SIZE);

            bool hit_wall = false;

            // Check current and next cells for walls (to account for diagonal gaps)
            if (cell_x >= 0 && cell_x < grid.width && cell_y >= 0 && cell_y < grid.height) {
                Cell& current_cell = grid.cells[cell_y * grid.width + cell_x];

                // Update luminance
                double luminosity = std::max(0.0, 1.0 - total_distance / max_distance);
                if (luminosity > current_cell.luminance) {
                    current_cell.luminance = luminosity;
                }

                // Check for wall hit in current cell
                if (current_cell.type == CellType::Wall) {
                    hit_wall = true;
                }
            }

            // Check next cells for diagonal wall detection
            if (!hit_wall && next_cell_x >= 0 && next_cell_x < grid.width && cell_y >= 0 && cell_y < grid.height &&
                grid.cells[cell_y * grid.width + next_cell_x].type == CellType::Wall) {
                hit_wall = true;
            }
            if (!hit_wall && cell_x >= 0 && cell_x < grid.width && next_cell_y >= 0 && next_cell_y < grid.height &&
                grid.cells[next_cell_y * grid.width + cell_x].type == CellType::Wall) {
                hit_wall = true;
            }

            if (hit_wall || cell_x < 0 || cell_x >= grid.width || cell_y < 0 || cell_y >= grid.height) {
                // We've hit a wall or left the grid
                break;
            }

            // Move to next position
            current_x += direction.x * (CELL_SIZE / 2.0);
            current_y += direction.y * (CELL_SIZE / 2.0);
            total_distance += CELL_SIZE / 2.0;
        }
    }
}

void draw_grid(const Grid& grid) {
    for (int y = 0; y < grid.height; ++y) {
        for (int x = 0; x < grid.width; ++x) {
            const Cell& cell = get_cell(grid, x, y);
            color c;
            int luminance = static_cast<int>(cell.luminance * 255);
            if (cell.type == CellType::Wall) {
                c = rgba_color(luminance, luminance, luminance, 255);
            } else {
                c = rgba_color(luminance, luminance, luminance, 100);
            }
            fill_rectangle(c, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE / 2, CELL_SIZE / 2);
        }
    }
}

