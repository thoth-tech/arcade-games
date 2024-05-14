#include "enemy.h"
#include "game.h"

enemy_data new_enemy(int x, int y)
{
    enemy_data result;

    // load in bitmap for the spider sprite
    bitmap enemy_pack = load_bitmap("spider", "spider.png");

    // specify how the bitmap is partitioned
    bitmap_set_cell_details(enemy_pack,32,32,6,4,24);

    // load in the animation script
    //n ote that the animation script includes a vector that will make the spiders x and y position change.
    // spider speed can be controlled by changing those vector values in the spider.txt animation file
    load_animation_script("spider_walk", "spider.txt");

    // assign the animation script to the enemy property
    result.animation = animation_script_named("spider_walk");

    // create the enemy sprite using the bitmap and animation script
    result.enemy_sprite = create_sprite(enemy_pack, result.animation);

    // set the sprite coordinates based on values given when new enemy created
    sprite_set_x(result.enemy_sprite, x);
    sprite_set_y(result.enemy_sprite, y);
    
    // keep track of x and y coords
    result.x_pos   = sprite_x(result.enemy_sprite);
    result.y_pos   = sprite_y(result.enemy_sprite);

    // keep track of location id (tile id)
    result.x_id    = result.x_pos/TILESIZE;
    result.y_id    = result.y_pos/TILESIZE;

    // sets next tile position/id for each direction
    result.up_next    = result.y_id - 1;
    result.down_next  = result.y_id + 1;
    result.left_next  = result.x_id - 1;
    result.right_next = result.x_id + 1;

    // initialize movement bools. Used to keep track of what direction spider is moving in
    for (int i = 0; i < 4; i++)
        result.dir[i] = 0;

    // set bitmap to be used for collision detection
    sprite_set_collision_bitmap(result.enemy_sprite, enemy_pack);

    return result;
}

void draw_enemy(const enemy_data &enemy_to_draw)
{
    draw_sprite(enemy_to_draw.enemy_sprite);
}

void update_enemy(enemy_data &enemy_to_update)
{
    update_sprite(enemy_to_update.enemy_sprite);
    update_sprite_animation(enemy_to_update.enemy_sprite);
}
