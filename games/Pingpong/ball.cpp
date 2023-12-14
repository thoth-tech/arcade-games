#include "splashkit.h"
#include <vector>
#include "ball.h"
#include <cmath>
#include <cstdlib>

using namespace std;
#define BALL_MIN_X_SPEED 7.0
#define BALL_MAX_X_SPEED 25.0
#define BALL_MIN_Y_SPEED 5.0
#define BALL_MAX_Y_SPEED 15.0



bitmap ball_bitmap(ball_type type)
{
    switch (type)
    {
    case COLORFUL:
        return bitmap_named("ball2");
    case SOCCER:
        return bitmap_named("soccerball");
   
    default:
        return bitmap_named("soccerball");
    }
}
ball_data new_ball(ball_type type)
{
    ball_data ball;
    bitmap default_bitmap = ball_bitmap(type);

    ball.ball_sprite = create_sprite(default_bitmap);

    ball.xspeed = 0; 
 ball.yspeed = 0; 
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
    ball.ball_top = sprite_y(ball.ball_sprite);
    ball.ball_bottom = ball.ball_top + sprite_height(ball.ball_sprite);
    ball.ball_left = sprite_x(ball.ball_sprite);
    ball.ball_right = ball.ball_left + sprite_width(ball.ball_sprite);
}
// Function to generate a random number within a range
double random_range(double min, double max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}
// Function to update the ball's motion
void ball_motion(ball_data &ball)
{
    ball.xspeed = random_range(BALL_MIN_X_SPEED, BALL_MAX_X_SPEED);
    ball.yspeed = random_range(BALL_MIN_Y_SPEED, BALL_MAX_Y_SPEED);
    

    // Set the ball's horizontal speed based on the angle
    sprite_set_dx(ball.ball_sprite, ball.xspeed);
    sprite_set_dy(ball.ball_sprite, ball.yspeed);
    //sprite_set_rotation(ball.ball_sprite, ball.angle);
}
