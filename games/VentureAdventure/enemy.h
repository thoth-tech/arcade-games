#ifndef _ENEMY
#define _ENEMY

#include "splashkit.h"

using namespace std;

struct enemy_data
{
    sprite      enemy_sprite;
    animation_script   animation;

    bool move[4];
    bool stopped;

    int id;
    int x_pos, y_pos;
    int x_id, y_id;

};

enemy_data new_enemy(int x, int y);

void draw_enemy(const enemy_data &enemy_to_draw);

void update_enemy(enemy_data &enemy_to_update);

#endif