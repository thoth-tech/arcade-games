#ifndef CAR_RACE_GAME
#define CAR_RACE_GAME

#include "splashkit.h"
#include <vector>
#include "car.h"

// Structure to store game data
struct game_data
{
    bool game_over;           // Flag indicating if the game is over
    sprite whitemarks[6];     // Array of sprites for white marks on the road
    car_data car;             // Player's car
    vector<car_data> cars;    // Vector of obstacle cars
    double score;             // Current score
};

// Function to create a new game
game_data new_game();

// Function to draw the game
void draw_game(game_data &game);

// Function to update the game state
void update_game(game_data &game);

// Function to remove cars that have gone out of range
void out_range(game_data &game);

// Function to check for collisions between the player's car and obstacle cars
void check_collisions(game_data &game);

#endif
