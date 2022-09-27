#ifndef _PLAYER
#define _PLAYER

#include "splashkit.h"

using namespace std;

enum move_kind
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct player_data
{
    sprite      player_sprite;
    move_kind   kind;
    animation_script   animation;

    bool move[4], walking;
    bool stopped;
    bool attacked;

    int gem;
    int bullet;
    int x_pos, y_pos;
    int x_prev, y_prev;
    int x_id, y_id;
    int up_next, down_next;
    int left_next, right_next;
    int next;
};

player_data new_player();

void draw_player(const player_data &player_to_draw);

void update_player(player_data &player_to_update);

#endif 