#include "./include/types.h"
#include "splashkit.h"


// Bresenham's line algorithm
void draw_line_on_grid(Grid& grid, int x1, int y1, int x2, int y2) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        set_cell(grid, x1, y1, CellType::Wall);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void setup_walls(Grid& grid) {
    // Create random walls
    for (int i = 0; i < 7; ++i) {
        int x1 = rnd(grid.width);
        int y1 = rnd(grid.height);
        int x2 = rnd(grid.width);
        int y2 = rnd(grid.height);
        draw_line_on_grid(grid, x1, y1, x2, y2);
    }

    // Create a border
    draw_line_on_grid(grid, 0, 0, grid.width - 1, 0);
    draw_line_on_grid(grid, 0, 0, 0, grid.height - 1);
    draw_line_on_grid(grid, grid.width - 1, 0, grid.width - 1, grid.height - 1);
    draw_line_on_grid(grid, 0, grid.height - 1, grid.width - 1, grid.height - 1);
}

int main() {
    open_window("Grid-based Ray Marching Demo", SCREEN_WIDTH, SCREEN_HEIGHT);

    Grid grid = create_grid();
    setup_walls(grid);

    while (!quit_requested()) {
        process_events();
        clear_screen(COLOR_BLACK);

        point_2d mouse = mouse_position();
        march_light(grid, mouse, NUM_RAYS, MAX_LIGHT_DISTANCE);

        draw_grid(grid);

        refresh_screen(60);
    }

    return 0;
}
