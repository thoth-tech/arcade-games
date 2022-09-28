#ifndef _BOX
#define _BOX

#include "splashkit.h"

enum box_kind
{
    BOX1,
};

struct box_data
{
    sprite box_sprite;
    box_kind kind;
    bitmap default_box;

    bool up_stopped, down_stopped;
    bool left_stopped, right_stopped;
    bool is_moving;

    int id;
    int r, h;
    int x_pos, y_pos;
    int x_id, y_id;
    int up_next, down_next;
    int left_next, right_next;
    int x_prev, y_prev;
    int next;
};

box_data new_box(int x, int y);

void draw_box(const box_data &draw);

void update_box(box_data &update, int x, int y);

#endif