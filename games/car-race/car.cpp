#include "splashkit.h"
#include <vector>
#include "car.h"
using namespace std;

// Function to get the bitmap for a car model
bitmap car_bitmap(car_model model)
{
    switch (model)
    {
    case RED:
        return bitmap_named("redcar");
    case YELLOW:
        return bitmap_named("yellowcar");
    case BLACK:
        return bitmap_named("blackcarsolo");
    case POLICE:
        return bitmap_named("policecarsolo");
    default:
        return bitmap_named("policecarsolo");
    }
}

// Function to create a new car
car_data new_car(car_model model, double speed, double x, double y)
{
    car_data result;
    bitmap default_bitmap = car_bitmap(model);

    // Set cell details and animation script for RED and YELLOW cars
    if (model == RED || model == YELLOW)
    {
        bitmap_set_cell_details(default_bitmap, 75, 120, 3, 1, 3);
        animation_script carAnimation = load_animation_script("carAnimation", "carAnimation.txt");
        result.car_sprite = create_sprite(default_bitmap, carAnimation);
    }
    else
    {
        result.car_sprite = create_sprite(default_bitmap);
    }

    result.speed = speed;
    result.model = model;

    sprite_set_x(result.car_sprite, x);
    sprite_set_y(result.car_sprite, y);
    sprite_set_dy(result.car_sprite, result.speed);

    return result;
}

// Function to switch the car's skin
void switch_car_skin(car_data &car)
{
    bitmap other = car_bitmap(YELLOW);
    bitmap_set_cell_details(other, 75, 120, 3, 1, 3);
    sprite_add_layer(car.car_sprite, other, "YELLOW");
    sprite_hide_layer(car.car_sprite, 1);

    if (car.model == RED)
    {
        car.model = YELLOW;
        sprite_hide_layer(car.car_sprite, 0);
        sprite_show_layer(car.car_sprite, 1);
    }
    else if (car.model == YELLOW)
    {
        car.model = RED;
        sprite_hide_layer(car.car_sprite, 1);
        sprite_show_layer(car.car_sprite, 0);
    }
}

// Function to draw the car
void draw_car(car_data &car)
{
    draw_sprite(car.car_sprite);
}

// Function to update the car's position
void update_car(car_data &car)
{
    update_sprite(car.car_sprite);
}
