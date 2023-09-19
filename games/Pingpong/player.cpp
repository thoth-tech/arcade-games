#include "splashkit.h"
#include "player.h"
using namespace std;

// Function to get the appropriate bitmap for the player's skin
bitmap player_bitmap(player_skin skin)
{
    switch (skin)
    {
    case PLAYER1:
        return bitmap_named("bstriker");
    case PLAYER2:
        return bitmap_named("rstriker");
    default:
        return bitmap_named("striker");
    }
}

// Function to create a new player with the given player type
player_data new_player(bool player_type)
{
    player_data result;
    result.player_type = player_type;
    
    // Set the default bitmap based on the player type
    bitmap default_bitmap = player_bitmap(PLAYER2);
    result.player_sprite = create_sprite(default_bitmap);
    result.score = 0;
    
    // Set the initial position of the player sprite based on the player type
    if (player_type)
    {
        sprite_set_x(result.player_sprite, 173);
        sprite_set_y(result.player_sprite, 262);
    }
    else
    {
        result.player_sprite = create_sprite(player_bitmap(PLAYER1));
        sprite_set_x(result.player_sprite, 1060);
        sprite_set_y(result.player_sprite, 262);
    }
    
    return result;
}

// Function to draw the player sprite on the screen
void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.player_sprite);
}

// Function to update the player sprite
void update_player(player_data &player_to_update)
{
    update_sprite(player_to_update.player_sprite);
}
