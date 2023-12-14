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
    case RED_PLAYER:
        return bitmap_named("rsoccer");
    case BLUE_PLAYER:
        return bitmap_named("bsoccer");
    default:
        return bitmap_named("striker");
    }
}

// Function to create a new player with the given player type
player_data new_player(player_skin skin, bool type)
{
    player_data result;
    result.player_type = type;
    result.skin = skin;
    
    // Set the default bitmap based on the player type
    bitmap default_bitmap = player_bitmap(skin);
    result.player_sprite = create_sprite(default_bitmap);
    result.score = 0;
    
    // Set the initial position of the player sprite based on the player type
    if (type)
    {
        sprite_set_x(result.player_sprite, 173);
        sprite_set_y(result.player_sprite, 262);
    }
    else
    {
        
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
void update_player(player_data &player)
{
    update_sprite(player.player_sprite);
    player.player_top = sprite_y(player.player_sprite);
    player.player_bottom = player.player_top + sprite_height(player.player_sprite);
    player.player_left = sprite_x(player.player_sprite);
    player.player_right = player.player_left + sprite_width(player.player_sprite);
}
