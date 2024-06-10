# Tutorial for Updated Car Race

## 1. Updating car.cpp

**Additions for Two Player Mode:** This update adds functionality to support a two-player mode in the game.

- **new_car function:** When creating a new car, it now also initializes a second player's car with a different color (YELLOW), positioned slightly to the right of the first player's car.

- **update_car function:** Updates the position of both player 1 and player 2 cars. If the game is paused (`game.paused` is true), it skips updating the cars.

```cpp
// Additions for Two Player Mode
car_data new_car(car_model model, double speed, double x, double y) {
    // ...
    result.secondPlayer = new_car(YELLOW, speed, x + 100, y);
    result.twoPlayer = true;
    // ...
}

void update_car(car_data &car) {
    // Pause the game if it's paused
    if (game.paused) return;

    // Update player 1 car
    update_sprite(car.car_sprite);

    // Update player 2 car
    if (car.twoPlayer) {
        update_sprite(car.secondPlayer.car_sprite);
    }
}
```

## 2. Updating car.h

**Additions for Two Player Mode:** Extends the `car_data` structure to support two-player mode.

- **secondPlayer:** Stores the data for the second player's car.
  
- **twoPlayer:** A boolean flag indicating whether the car is controlled by the second player.
```cpp
// Additions for Two Player Mode
struct car_data {
    // ...
    car_data secondPlayer;
    bool twoPlayer;
    // ...
};
```

## 3. Updating game.h

**Additions for Two Player Mode:** Extends the `game_data` structure to accommodate two-player mode.

- **secondPlayer:** Stores the data for the second player's car.
  
- **twoPlayer:** A boolean flag indicating whether the game is in two-player mode.
  
- **paused:** A boolean flag indicating whether the game is currently paused.
  
- **finished:** A boolean flag indicating whether the game has finished.
  
- **finish_line:** Stores the sprite for the finish line.
```cpp
// Additions for Two Player Mode
struct game_data {
    // ...
    car_data secondPlayer;
    bool twoPlayer;
    bool paused;
    bool finished;
    sprite finish_line;
};
```

## 4. Updating powerup.cpp
- powerup_bitmap Function: This function takes a powerup_type enum as input and returns the corresponding bitmap for the given powerup type. It uses a switch statement to select the appropriate bitmap based on the powerup type.

- new_powerup Function: This function creates a new powerup with the specified type, speed, and position (x, y). It initializes a sprite for the powerup using the powerup_bitmap function, sets its position, type, and speed, and returns the resulting powerup_data structure.

- draw_powerup Function: This function draws the powerup sprite on the screen.

- update_powerup Function: This function updates the position of the powerup sprite by moving it vertically downwards based on its speed.

- out_range_powerup Function: This function removes powerups from the vector powerups that have gone out of the visible range of the screen. It iterates through the vector and checks the y-coordinate of each powerup sprite. If it exceeds a certain threshold (600 in this case), the powerup is removed from the vector.

```cpp
#include "splashkit.h"
#include <vector>
#include "powerup.h"

int main()
{
    // Initialize SplashKit
    open_window("Powerup Demo", 800, 600);
    load_resource_bundle("powerup_bundle", "powerup_bundle.txt");

    // Create a vector to store powerups
    std::vector<powerup_data> powerups;

    // Add some powerups to the vector
    powerups.push_back(new_powerup(SHIELD, 5, 100, 100));
    powerups.push_back(new_powerup(SPEED_BOOST, 3, 200, 200));
    powerups.push_back(new_powerup(COIN, 4, 300, 300));

    // Main game loop
    while (not quit_requested())
    {
        // Update powerups
        for (auto &powerup : powerups)
        {
            update_powerup(powerup);
        }

        // Clear the screen
        clear_screen(COLOR_WHITE);

        // Draw powerups
        for (auto &powerup : powerups)
        {
            draw_powerup(powerup);
        }

        // Remove out-of-range powerups
        out_range_powerup(powerups);

        // Refresh the screen
        refresh_screen();
        delay(17); // Cap the frame rate to approximately 60 frames per second
    }

    // Close SplashKit
    close_window("Powerup Demo");
    return 0;
}

```

## 5. Updating powerup.h
- The explanation of code as follows :
- powerup.h defines the structure and functions related to powerups in the game.
- powerup_data struct stores data for each powerup, including its sprite, type, and speed.
- The powerup_type enum lists different types of powerups available in the game.
- new_powerup function creates a new powerup with the specified type, speed, and position.
- draw_powerup function draws the powerup sprite on the screen.
- update_powerup function updates the powerup's position based on its speed.
- out_range_powerup function removes powerups that have gone out of range from the screen.
```cpp
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

```


## 6. Updating barricade.cpp
- Header Inclusions: Includes necessary headers such as "splashkit.h" for SplashKit functionalities and <vector> for vector data structure.
- Constants and Variables: Defines constants like gap_size and an array gap_locations to store possible positions for gaps in the barricades. previous_gap_location is initialized to an invalid value to ensure that the first gap location is chosen randomly.
- get_new_gap_location Function: Generates a random index to select a gap location from the gap_locations array, ensuring that the same gap location is not chosen consecutively.
- barricade_bitmap Function: Creates a bitmap representing a barricade with a gap. It selects a random gap location, draws the left and right parts of the barricade around the gap, sets up collision masks, and returns the resulting bitmap.
- new_barricade Function: Creates a new barricade with the specified speed and y-coordinate. It initializes a sprite using the bitmap generated by barricade_bitmap and sets its position and speed.
- draw_barricade Function: Draws the barricade sprite on the screen.
- update_barricade Function: Updates the position of the barricade sprite.
- out_range_barricade Function: Removes barricades from the vector barricades that have gone out of the visible range of the screen.

```cpp
#include "splashkit.h"
#include <vector>
#include "barricade.h"

// Define constants
#define gap_size 100
double gap_locations[] = {250, 300, 350, 400, 450, 500, 550};
double previous_gap_location = -1;

// Function to get a new random gap location
double get_new_gap_location()
{
    int index;
    do
    {
        index = rand() % 7;
    } while (gap_locations[index] == previous_gap_location);

    previous_gap_location = gap_locations[index];
    return gap_locations[index];
}

// Function to create a bitmap for a barricade with a gap
bitmap barricade_bitmap()
{
    bitmap barricade_bitmap = create_bitmap("barricade3", 750, 40);
    double gap_location = get_new_gap_location();

    draw_bitmap_on_bitmap(barricade_bitmap, bitmap_named("barricade2"), 0, 0, option_part_bmp(0, 0, gap_location, 40));
    draw_bitmap_on_bitmap(barricade_bitmap, bitmap_named("barricade2"), gap_location + gap_size, 0, option_part_bmp(gap_location + gap_size, 0, 700 - gap_location - gap_size, 40));
    
    setup_collision_mask(barricade_bitmap);
    return barricade_bitmap;
}

// Function to create a new barricade
barricade_data new_barricade(double speed, double y)
{
    barricade_data result;
    result.barricade_sprite = create_sprite(barricade_bitmap());
    sprite_set_x(result.barricade_sprite, 50);
    sprite_set_y(result.barricade_sprite, y);
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
            barricades.erase(barricades.begin() + j);
            j--;
        }
    }
}


```

## 7. Updating barricade.h
- The explanation of code as follows : 
- barricade.h defines the structure and functions related to barricades in the game.
- barricade_data struct stores data for each barricade, including its sprite, speed, and gap location.
- new_barricade function creates a new barricade with the specified speed and initial position.
- draw_barricade function draws the barricade sprite on the screen.
- update_barricade function updates the barricade's position based on its speed.
- out_range_barricade function removes barricades that have gone out of range from the screen.

```cpp
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

```
## 8. Conclusion
In conclusion, these code files provide a foundation for creating a dynamic and engaging car racing game with features such as multiple car models, powerups, obstacles, and two-player mode. By utilizing SplashKit's functionality and these code structures, developers can easily extend and customize the game to suit their preferences and requirements.