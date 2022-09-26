#ifndef PADDLE
#define PADDLE

#define WIDTH 10
#define LENGTH 100

#include "splashkit.h"

using namespace std;

struct paddle_data
{
    float x, y;
    int w, l;
    color color;
    string name;
    sprite paddle_sprite;
};

paddle_data new_paddle(int x, int y);

void draw_paddle(const paddle_data &paddle);
void update_paddle(paddle_data &paddle);

void controls_p1(paddle_data &paddle);
void controls_p2(paddle_data &paddle);

#endif