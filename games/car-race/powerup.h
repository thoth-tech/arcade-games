#ifndef POWER_UP
#define POWER_UP

#include "splashkit.h"
#include <vector>

// Enumeration for different powerups
enum powerup_type
{
    SHIELD,
    SPEED_BOOST,
    SLOW_DOWN,
    COIN
};

// Structure to store powerup data
struct powerup_data
{
    sprite powerup_sprite; // Sprite representing the powerup
    powerup_type type;     // Type of the powerup
    double speed;          // Speed of the powerup
};

// Function to create a new powerup
powerup_data new_powerup(powerup_type type, double speed, double x, double y);

// Function to draw the powerup
void draw_powerup(powerup_data &powerup);

// Function to update the powerup's position
void update_powerup(powerup_data &powerup);

// Function to remove powerups that have gone out of range
void out_range_powerup(std::vector<powerup_data> &powerups);

#endif