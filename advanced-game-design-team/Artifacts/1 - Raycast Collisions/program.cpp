#include "./include/types.h"
#include "splashkit.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int NUM_RAYS = 360;
const double MAX_DISTANCE = 1000;
const int NUM_OBSTACLES = 30;

std::vector<rectangle> generate_random_obstacles(int num_obstacles) {
    std::vector<rectangle> obstacles;
    for (int i = 0; i < num_obstacles; ++i) {
        double x = rnd(SCREEN_WIDTH);
        double y = rnd(SCREEN_HEIGHT);
        double width = rnd(10, 300);
        double height = rnd(10, 20);
        obstacles.push_back(rectangle_from(x, y, width, height));
    }
    return obstacles;
}

int main() {
    open_window("Ray Marching Demo", SCREEN_WIDTH, SCREEN_HEIGHT);

    std::vector<rectangle> obstacles = generate_random_obstacles(NUM_OBSTACLES);

    while (!quit_requested()) {
        process_events();
        clear_screen(COLOR_BLACK);

        point_2d mouse = mouse_position();
        std::vector<ray_hit> hits = cast_rays(obstacles, mouse, NUM_RAYS, MAX_DISTANCE);

        draw_obstacles(obstacles, COLOR_GRAY);
        draw_rays(hits, mouse, COLOR_WHITE);

        refresh_screen(60);
    }

    return 0;
}
