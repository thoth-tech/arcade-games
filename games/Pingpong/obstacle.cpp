#include "splashkit.h"
#include "obstacle.h"

// Function to retrieve the appropriate bitmap for the obstacle kind
bitmap obstacle_bitmap(obstacle_kind kind)
{
    switch (kind)
    {
        case VERTICAL:
            return bitmap_named("vertical");
        case HORIZONTAL:
            return bitmap_named("horizontal");
        default:
            return bitmap_named("horizontal");
    } 
}

// Function to create a new obstacle with the given kind, x and y coordinates
obstacle_data new_obstacle(obstacle_kind kind, double x, double y)
{
    obstacle_data obstacle;
    bitmap default_bitmap = obstacle_bitmap(kind);
    obstacle.obstacle_sprite = create_sprite(default_bitmap);
    obstacle.kind = kind;
    sprite_set_x(obstacle.obstacle_sprite, x);
    sprite_set_y(obstacle.obstacle_sprite, y);
    return obstacle;
}

// Function to draw the obstacle on the screen
void draw_obstacle(obstacle_data &obstacle)
{
    draw_sprite(obstacle.obstacle_sprite);
}

// Function to update the obstacle's position
void update_obstacle(obstacle_data &obstacle)
{
    update_sprite(obstacle.obstacle_sprite);
}