#include "splashkit.h"
#include <vector>
#include "powerup.h"



bitmap powerup_bitmap(powerup_type type)
{
    switch (type)
    {
    case SHIELD:
        return bitmap_named("shield");
    case SPEED_BOOST:
        return bitmap_named("speedup");
    case SLOW_DOWN:
        return bitmap_named("slowdown");
    case COIN:
        return bitmap_named("coin");
    default:
        return bitmap_named("coin");
    }
    
}
// Function to create a new powerup
powerup_data new_powerup(powerup_type type, double speed, double x, double y)
{
    powerup_data result;
    //random powerup
    // powerup_type type = static_cast<powerup_type>(rand() % 3);
    result.powerup_sprite = create_sprite(powerup_bitmap(type));
   
    sprite_set_x(result.powerup_sprite, x);
    sprite_set_y(result.powerup_sprite, y);
    result.type = type;
    result.speed = speed;
    return result;
}

// Function to draw the powerup
void draw_powerup(powerup_data &powerup)
{
    draw_sprite(powerup.powerup_sprite);
}

// Function to update the powerup's position
void update_powerup(powerup_data &powerup)
{
    sprite_set_y(powerup.powerup_sprite, sprite_y(powerup.powerup_sprite) + powerup.speed);
}

// Function to remove powerups that have gone out of range
void out_range_powerup(std::vector<powerup_data> &powerups)
{
    for (int j = 0; j < powerups.size(); j++)
    {
        if (sprite_y(powerups[j].powerup_sprite) > 600)
        {
            // Remove the element from the vector
            powerups.erase(powerups.begin() + j);
            // Decrement j to account for the removed element
            j--;
        }
    }
}

