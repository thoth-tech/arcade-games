#ifndef CAR_RACE_CAR
#define CAR_RACE_CAR

#include "splashkit.h"
#include <vector>

// Enumeration for different car models
enum car_model
{
    RED,
    YELLOW,
    BLACK,
    POLICE
};

// Structure to store car data
struct car_data
{
    sprite car_sprite; // Sprite representing the car
    car_model model;   // Model of the car
    double speed;      // Speed of the car
};

// Function to create a new car
car_data new_car(car_model model, double speed, double x, double y);

// Function to draw the car
void draw_car(car_data &car);

// Function to switch the car's skin
void switch_car_skin(car_data &car);

// Function to update the car's position
void update_car(car_data &car);

#endif
