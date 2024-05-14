#ifndef _ENEMY
#define _ENEMY

#include "splashkit.h"

using namespace std;

enum dir_kind
{
    EUP,
    EDOWN,
    ELEFT,
    ERIGHT,
};
struct enemy_data
{
    sprite      enemy_sprite;
    dir_kind   kind;
    animation_script   animation;

    bool dir[4];
    bool stopped;

    int x_prev, y_prev;
    int up_next, down_next;
    int left_next, right_next;

    int id;
    int x_pos, y_pos;
    int x_id, y_id;

};

enemy_data new_enemy(int x, int y);

void draw_enemy(const enemy_data &enemy_to_draw);

void update_enemy(enemy_data &enemy_to_update);

#endif