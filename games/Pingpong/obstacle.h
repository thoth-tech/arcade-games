#ifndef PING_PONG_OBSTACLE
#define PING_PONG_OBSTACLE
#include "splashkit.h"
// Enum to represent the kind of obstacle
enum obstacle_kind
{
VERTICAL,
HORIZONTAL
};
// Struct to store obstacle data
struct obstacle_data
{
obstacle_kind kind;
sprite obstacle_sprite;
};
// Function to create a new obstacle
obstacle_data new_obstacle(obstacle_kind kind, double x, double y);
// Function to draw the obstacle on the screen
void draw_obstacle(obstacle_data &obstacle);
// Function to update the obstacle's position
void update_obstacle(obstacle_data &obstacle);
#endif  // PING_PONG_OBSTACLE