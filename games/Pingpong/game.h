// This code defines a header file for a ping pong game.
#ifndef PING_PONG_GAME
#define PING_PONG_GAME
#include "splashkit.h" // Include the SplashKit library for game development
#include <vector>      // Include the vector library for managing collections of objects
#include "player.h"    // Include the header file for the player object
#include "ball.h"      // Include the header file for the ball object
#include "obstacle.h"  // Include the header file for the obstacle object
// Define a struct to hold the game data
struct game_data
{
    player_data player1; // Data for player 1
    player_data player2; // Data for player 2
    ball_data ball;      // Data for the ball

    vector<obstacle_data> obstacles; // Collection of obstacle data

    int screen;        // Current screen of the game
    bool intro_played; // Flag to indicate if the game intro has been played
    bool started;      // Flag to indicate if the game has started
    bool ended;        // Flag to indicate if the game has ended
};
// Function to create a new game and return the game data
game_data new_game();
// Function to draw the game on the screen
void draw_game(game_data &game);
// Function to update the game state
void update_game(game_data &game);
// Function to draw the end screen of the game
void draw_end(game_data &game);
#endif // End of PING_PONG_GAME ifndef directive