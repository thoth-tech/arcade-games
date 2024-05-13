#ifndef BARRICADE
#define BARRICADE

#include "splashkit.h"
#include <vector>

// Structure to store barricade data
struct barricade_data
{
    sprite barricade_sprite; // Sprite representing the barricade
    double speed;            // Speed of the barricade
    double gap_location;     // Location of the gap in the barricade
};

// Function to create a new barricade
barricade_data new_barricade(double speed, double y);

// Function to draw the barricade
void draw_barricade(barricade_data &barricade);

// Function to update the barricade's position
void update_barricade(barricade_data &barricade);

// Function to remove barricades that have gone out of range
void out_range_barricade(std::vector<barricade_data> &barricades);

#endif
