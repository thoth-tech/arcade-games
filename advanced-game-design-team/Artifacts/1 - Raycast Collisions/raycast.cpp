#include "./include/types.h"
#include <algorithm>
#include <limits>

ray create_ray(point_2d origin, vector_2d direction) {
    return {origin, unit_vector(direction)};
}

// Slab / Ray-AABB rect intsersection
bool ray_rectangle_intersection(const ray& r, const rectangle& rect, ray_hit& hit) {
    vector_2d ray_vector = vector_to(r.direction.x, r.direction.y);
    if (vector_in_rect(ray_vector, rect)) {
        hit.hit_point = r.origin;
        hit.distance = 0;
        hit.hit_obstacle = true;
        return true;
    }

    double t_near = -std::numeric_limits<double>::infinity();
    double t_far = std::numeric_limits<double>::infinity();

    vector_2d inv_dir = vector_to(1.0 / r.direction.x, 1.0 / r.direction.y);

    double t1x = (rect.x - r.origin.x) * inv_dir.x;
    double t2x = (rect.x + rect.width - r.origin.x) * inv_dir.x;
    double t1y = (rect.y - r.origin.y) * inv_dir.y;
    double t2y = (rect.y + rect.height - r.origin.y) * inv_dir.y;

    t_near = std::max(std::min(t1x, t2x), std::min(t1y, t2y));
    t_far = std::min(std::max(t1x, t2x), std::max(t1y, t2y));

    if (t_near > t_far || t_far < 0) return false;

    hit.distance = t_near;
    vector_2d hit_vector = vector_multiply(r.direction, t_near);
    hit.hit_point = point_at(r.origin.x + hit_vector.x, r.origin.y + hit_vector.y);
    hit.hit_obstacle = true;
    return true;
}

// not a real march, just using Ray-AABB intersection checks without marching through space
ray_hit march_ray(const ray& r, const std::vector<rectangle>& obstacles, double max_distance) {
    vector_2d end_vector = vector_multiply(r.direction, max_distance);
    point_2d end_point = point_at(r.origin.x + end_vector.x, r.origin.y + end_vector.y);
    ray_hit closest_hit = {end_point, max_distance, false};

    for (const auto& obs : obstacles) {
        ray_hit hit;
        if (ray_rectangle_intersection(r, obs, hit) && hit.distance < closest_hit.distance) {
            closest_hit = hit;
        }
    }

    return closest_hit;
}

std::vector<ray_hit> cast_rays(const std::vector<rectangle>& obstacles, point_2d origin, int num_rays, double max_distance) {
    std::vector<ray_hit> hits;
    hits.reserve(num_rays);

    for (int i = 0; i < num_rays; ++i) {
        double angle = i * (360.0 / num_rays);
        vector_2d direction = vector_from_angle(angle, 1.0);
        ray r = create_ray(origin, direction);
        hits.push_back(march_ray(r, obstacles, max_distance));
    }

    return hits;
}

void draw_rays(const std::vector<ray_hit>& hits, const point_2d& origin, color c) {
    for (const auto& hit : hits) {
        draw_line(c, origin, hit.hit_point);
    }
}

void draw_obstacles(const std::vector<rectangle>& obstacles, color c) {
    for (const auto& obs : obstacles) {
        fill_rectangle(c, obs);
    }
}
