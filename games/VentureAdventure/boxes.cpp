#include "boxes.h"
#include "game.h"
#include "splashkit.h"

using namespace std;

bitmap box_image(box_kind p_up)
{
    switch (p_up)
    {
        case BOX1:
            return bitmap_named("box4");
            break;
        default:
            return bitmap_named("box4");
    }
}

box_data new_box(int x, int y)
{
    box_data result;

    result.kind = static_cast<box_kind>(BOX1);

    result.box_sprite = create_sprite(box_image(result.kind));

    result.x_pos = x;
    result.y_pos = y;

    sprite_set_x(result.box_sprite, result.x_pos);
    sprite_set_y(result.box_sprite, result.y_pos);

    result.x_id    = result.x_pos/TILESIZE;
    result.y_id    = result.y_pos/TILESIZE;

    result.up_next    = result.y_id - 1;
    result.down_next  = result.y_id + 1;
    result.left_next  = result.x_id - 1;
    result.right_next = result.x_id + 1;

    result.up_stopped = false, result.down_stopped = false;
    result.left_stopped = false, result.right_stopped = false;

    result.is_moving  = false;

    result.r = TILESIZE, result.h = TILESIZE;

    return result;
}

void draw_box(const box_data &draw)
{
    draw_sprite(draw.box_sprite);
}

void update_box(box_data &update, int x, int y)
{
    update_sprite(update.box_sprite);
}


