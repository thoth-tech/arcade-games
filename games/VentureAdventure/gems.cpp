#include "gems.h"
#include "splashkit.h"

using namespace std;

string gem_image(gem_kind gem)
{
    switch (gem)
    {
        case SAPPHIRE:
            return "sapphire";
            break;
        case EMERALD:
            return "emerald";
            break;
        case RUBY:
            return "ruby";
            break;
        case DIAMOND:
            return "diamond";
            break;
        default:
            return "diamond";
    }
}

gem_data new_gem(int x, int y)
{
    gem_data result;

    bitmap gem_pack = load_bitmap("gems", "gems.png");
    bitmap_set_cell_details(gem_pack,32,32, 2, 2, 4);

    load_animation_script("gem", "gems.txt");
    result.anim = animation_script_named("gem");
    
    result.gem_sprite = create_sprite(gem_pack, result.anim);

    result.x = x;
    result.y = y;

    sprite_set_x(result.gem_sprite, result.x);
    sprite_set_y(result.gem_sprite, result.y);

    sprite_set_collision_bitmap(result.gem_sprite, gem_pack);
    sprite_start_animation(result.gem_sprite, DIAMOND);

    return result;
}

void draw_gem(const gem_data &draw)
{
    draw_sprite(draw.gem_sprite);
}

void update_gem(gem_data &update, int x, int y)
{
    update_sprite(update.gem_sprite);
}