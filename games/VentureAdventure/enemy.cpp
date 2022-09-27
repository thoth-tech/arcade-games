#include "enemy.h"
#include "game.h"

enemy_data new_enemy(int x, int y)
{
    enemy_data result;

    bitmap enemy_pack = load_bitmap("spider", "spider.png");
    bitmap_set_cell_details(enemy_pack,32,32,6,4,24);

    load_animation_script("spider_walk", "spider.txt");
    result.animation = animation_script_named("spider_walk");

    result.enemy_sprite = create_sprite(enemy_pack, result.animation);

    sprite_set_x(result.enemy_sprite, x);
    sprite_set_y(result.enemy_sprite, y);
    
    result.x_pos   = sprite_x(result.enemy_sprite);
    result.y_pos   = sprite_y(result.enemy_sprite);

    result.x_id    = result.x_pos/TILESIZE;
    result.y_id    = result.y_pos/TILESIZE;

    for (int i = 0; i < 4; i++)
        result.move[i] = 0;

    sprite_set_collision_bitmap(result.enemy_sprite, enemy_pack);
    sprite_start_animation(result.enemy_sprite, "s_left");

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
