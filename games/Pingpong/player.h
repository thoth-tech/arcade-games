#ifndef PING_PONG_PLAYER
#define PING_PONG_PLAYER
#include "splashkit.h"
#include <vector>
// Enumeration for different player skins
enum player_skin
{
PLAYER1,
PLAYER2,
RED_PLAYER,
BLUE_PLAYER
};
// Structure to store player data
struct player_data {
bool player_type;  // false for player 1, true for player 2
player_skin skin;  // the skin of the player
sprite player_sprite;  // the sprite of the player
int score;  // the score of the player
double player_top, player_bottom, player_right, player_left;
};
// Function to create a new player
player_data new_player(player_skin skin, bool type);
// Function to update the player
void update_player(player_data &player_to_update);
// Function to get the bitmap of the player skin
bitmap player_bitmap(player_skin skin);
// Function to draw the player
void draw_player(const player_data &player_to_draw);
#endif  // PING_PONG_PLAYER