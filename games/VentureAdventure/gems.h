#ifndef _GEMS
#define _GEMS

#include "splashkit.h"

using namespace std;

enum gem_kind
{
    SAPPHIRE,
    EMERALD,
    RUBY,
    DIAMOND
};

struct gem_data
{
    sprite gem_sprite;
    animation_script anim;
    int x, y;
    int x_id, y_id;
    int id;
};

gem_data new_gem(int x, int y);

void draw_gem(const gem_data &draw);

void update_gem(gem_data &update, int x, int y);

#endif