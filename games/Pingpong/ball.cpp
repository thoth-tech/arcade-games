#include "splashkit.h"
#include <vector>
#include "ball.h"
#include <cmath>
#include <cstdlib>

using namespace std;
#define BALL_MIN_SPEED 7.0
#define BALL_MAX_SPEED 25.0
#define BALL_MIN_ANGLE 60
#define BALL_MAX_ANGLE 300



bitmap ball_bitmap(ball_type type)
{
    switch (type)
    {
    case COLORFUL:
        return bitmap_named("ball2");
    default:
        return bitmap_named("ball2");
    }
}
ball_data new_ball(ball_type type)
{
    ball_data ball;
    bitmap default_bitmap = ball_bitmap(type);

    ball.ball_sprite = create_sprite(default_bitmap);

    ball.speed = 0; 

    sprite_set_x(ball.ball_sprite, (screen_width() / 2)-25);
    sprite_set_y(ball.ball_sprite, (screen_height() / 2)-25);

    return ball;
}
void draw_ball(ball_data ball)
{
    draw_sprite(ball.ball_sprite);
}
void update_ball(ball_data &ball)
{
    update_sprite(ball.ball_sprite);
}
// Function to generate a random number within a range
double random_range(double min, double max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}
// Function to update the ball's motion
void ball_motion(ball_data &ball)
{
    ball.speed = random_range(BALL_MIN_SPEED, BALL_MAX_SPEED);
    ball.angle = random_range(BALL_MIN_ANGLE, BALL_MAX_ANGLE);
    if(ball.angle == 0 || ball.angle == 180|| ball.angle == 90) ball.angle = 45;

    // Set the ball's horizontal speed based on the angle
    sprite_set_dx(ball.ball_sprite, ball.speed);
    sprite_set_rotation(ball.ball_sprite, ball.angle);
}
