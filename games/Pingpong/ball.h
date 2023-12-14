#ifndef PING_PONG_BALL
#define PING_PONG_BALL
#include "splashkit.h"
#include <vector>
// Enum to represent different types of balls
enum ball_type
{
    COLORFUL,
    SOCCER
};
// Struct to store data of a ball
struct ball_data
{
    sprite ball_sprite; // Sprite object representing the ball
    double xspeed;       // Speed of the ball in x
    double yspeed;       // Speed of the ball in y
    double ball_top;
    double ball_bottom;
    double ball_left;
    double ball_right;
};
// Function to get the bitmap for a given ball type
bitmap ball_bitmap(ball_type type);
// Function to create a new ball with the given type
ball_data new_ball(ball_type type);
// Function to draw the ball on the screen
void draw_ball(ball_data ball);
// Function to update the position of the ball
void update_ball(ball_data &ball);
// Function to handle the motion of the ball
void ball_motion(ball_data &ball);
#endif