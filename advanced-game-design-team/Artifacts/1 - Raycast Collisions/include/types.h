#ifndef TYPES_H
#define TYPES_H

#include "splashkit.h"
#include <vector>

struct ray {
    point_2d origin;
    vector_2d direction;
};

struct ray_hit {
    point_2d hit_point;
    double distance;
    bool hit_obstacle;
};

ray create_ray(point_2d origin, vector_2d direction);
std::vector<ray_hit> cast_rays(const std::vector<rectangle>& obstacles, point_2d origin, int num_rays, double max_distance);
void draw_rays(const std::vector<ray_hit>& hits, const point_2d& origin, color c);
void draw_obstacles(const std::vector<rectangle>& obstacles, color c);

#endif // TYPES_H
