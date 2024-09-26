#ifndef TYPES_H
#define TYPES_H

#include "splashkit.h"
#include <vector>

const int RES = 150; // Number of cells in each dimension
const int MAX_LIGHT_DISTANCE = 500;
const double SCREEN_WIDTH = 1000;
const double SCREEN_HEIGHT = 1000;
const double CELL_SIZE = SCREEN_WIDTH / RES;
const int NUM_RAYS = 700;

enum class CellType {
    Floor,
    Wall
};

struct Cell {
    CellType type;
    double luminance;
};

struct Grid {
    std::vector<Cell> cells;
    int width;
    int height;
};

struct Ray {
    point_2d origin;
    vector_2d direction;
};

struct RayHit {
    point_2d hit_point;
    double distance;
    bool hit_wall;
};

Grid create_grid();
void set_cell(Grid& grid, int x, int y, CellType type);
Cell get_cell(const Grid& grid, int x, int y);
std::vector<RayHit> cast_rays(const Grid& grid, point_2d origin, int num_rays, double max_distance);
void update_luminance(Grid& grid, const std::vector<RayHit>& hits, const point_2d& light_source);
void draw_grid(const Grid& grid);
void march_light(Grid& grid, point_2d light_source, int num_rays, double max_distance);

#endif // TYPES_H
