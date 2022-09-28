#include "player.h"
#include "game.h"
#include "splashkit.h"

class Player{
    private:
    public:
        sprite player_sprite;
        animation_script animation;
        bitmap hero_pack;

        bool move[4];
        bool walking;
        bool stopped;
        bool attacked;
        bool attacking;

        int x_pos, y_pos;
        int x_prev, y_prev;
        int x_id, y_id;
        int up_next, down_next;
        int left_next, right_next;
        int next;

        Player()
        {
            // load in the hero bitmap to be used for hero sprite
            hero_pack = load_bitmap("hero", "hero7.png");
            
            // specify how the bitmap is partitioned
            bitmap_set_cell_details(hero_pack,32,32,3,4,12);

            // load in animation script
            load_animation_script("hero_walk", "hero.txt");

            // assign the animation script to the player property
            animation = animation_script_named("hero_walk");

            // create the player sprite using the hero bitmap and animation script
            player_sprite = create_sprite(hero_pack, animation);
        }


};

player_data new_player()
{
    player_data result;

    // load in the hero bitmap to be used for hero sprite
    bitmap hero_pack = load_bitmap("hero", "hero7.png");

    // specify how the bitmap is partitioned
    bitmap_set_cell_details(hero_pack,32,32,3,4,12);

    // load in animation script
    load_animation_script("hero_walk", "hero.txt");

    // assign the animation script to the player property
    result.animation = animation_script_named("hero_walk");

    // create the player sprite using the hero bitmap and animation script
    result.player_sprite = create_sprite(hero_pack, result.animation);

    // set player in centre of map
    sprite_set_x(result.player_sprite, SCREEN_WIDTH/2);
    sprite_set_y(result.player_sprite, SCREEN_HEIGHT/2);
    
    // set player co-ords
    result.x_pos   = sprite_x(result.player_sprite);
    result.y_pos   = sprite_y(result.player_sprite);

    // set player location id
    result.x_id    = result.x_pos/TILESIZE;
    result.y_id    = result.y_pos/TILESIZE;

    // set player next postion for each location
    result.up_next    = result.y_id - 1;
    result.down_next  = result.y_id + 1;
    result.left_next  = result.x_id - 1;
    result.right_next = result.x_id + 1;
    result.x_prev  = 0, result.y_prev  = 0;

    // initalise gems and bullets
    result.gem     = 0;
    result.bullet  = 0;

    result.walking  = false;
    result.attacked = false;

    // initalise all movement bools
    for (int i = 0; i < 4; i++)
        result.move[i] = 0;

    // set the bitmap to be used for sprite collisions
    sprite_set_collision_bitmap(result.player_sprite, hero_pack);

    // initalise player standing still
    sprite_start_animation(result.player_sprite, "stand_d");

    return result;
}

void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.player_sprite);
}

void update_player(player_data &player_to_update)
{
    update_sprite(player_to_update.player_sprite);
    update_sprite_animation(player_to_update.player_sprite);
}