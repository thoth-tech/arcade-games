#include "splashkit.h"
#include <vector>
#include "barricade.h"

#define gap_size 150
double gap_locations[] = {250, 300, 350, 400, 450, 500, 550};
double previous_gap_location = -1; // Initialize to an invalid value

double get_new_gap_location()
{
    int index;
    do
    {
        index = rand() % 7; // Generate a random index between 0 and 3
    } while (gap_locations[index] == previous_gap_location);

    previous_gap_location = gap_locations[index];
    return gap_locations[index];
}

// Function to get the bitmap for a barricade
bitmap barricade_bitmap()
{
    // Adjust the gap size as needed
    bitmap barricade_bitmap = create_bitmap("barricade3", 750, 40); // Adjust the size of the barricade as needed

    // get random location between 50 and 650

    double gap_location = get_new_gap_location();
    // write_line("gap location: " + std::to_string(gap_location));
    // Draw the left part of the barricade
    draw_bitmap_on_bitmap(barricade_bitmap, bitmap_named("barricade2"), 0, 0, option_part_bmp(0, 0, gap_location, 40));

    // Draw the right part of the barricade
    draw_bitmap_on_bitmap(barricade_bitmap, bitmap_named("barricade2"), gap_location + gap_size, 0, option_part_bmp(gap_location + gap_size, 0, 700 - gap_location - gap_size, 40));
    //setup_collision_mask(bitmap bmp)
    setup_collision_mask(barricade_bitmap);

    // save_bitmap(barricade_bitmap, "barricade2");
    return barricade_bitmap;
}

// Function to create a new barricade
barricade_data new_barricade(double speed, double y)
{
    barricade_data result;
    // bitmap bitmap_with_gap = barricade_bitmap();
    result.barricade_sprite = create_sprite(barricade_bitmap());
    sprite_set_x(result.barricade_sprite, 50);
    sprite_set_y(result.barricade_sprite, y);

    result.speed = speed;
    sprite_set_dy(result.barricade_sprite, speed);
    return result;
}

// Function to draw the barricade
void draw_barricade(barricade_data &barricade)
{

    draw_sprite(barricade.barricade_sprite);
}

// Function to update the barricade's position
void update_barricade(barricade_data &barricade)
{
    update_sprite(barricade.barricade_sprite);
}

// Function to remove barricades that have gone out of range
void out_range_barricade(std::vector<barricade_data> &barricades)
{
    for (int j = 0; j < barricades.size(); j++)
    {
        if (sprite_y(barricades[j].barricade_sprite) > 600)
        {
            // Remove the element from the vector
            barricades.erase(barricades.begin() + j);
            // Decrement j to account for the removed element
            j--;
        }
    }
}
// #endif