#include "splashkit.h"
#include "paddle.h"

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH  800

using namespace std;

color background = COLOR_BLACK;

struct ball_data
{
    double x;
    double y;
    double dx;
    double dy;
    sprite ball_sprite;
};

ball_data new_ball()
{
    ball_data result;

    bitmap ball = load_bitmap("ball", "ball.png");
    result.ball_sprite = create_sprite(ball);

    result.x = SCREEN_WIDTH  / 2;
    result.y = SCREEN_HEIGHT / 2;

    sprite_set_x(result.ball_sprite, result.x);
    sprite_set_y(result.ball_sprite, result.y);

    return result;
};

void draw_ball(const ball_data &ball)
{   
    draw_sprite(ball.ball_sprite);
}

void update_ball(ball_data &ball)
{
    update_sprite(ball.ball_sprite);
}

void load_resources()
{
    load_resource_bundle("bundle", "bundle.txt");
}

void score(ball_data &ball, int x, int y)
{
    ball.x = SCREEN_WIDTH  / 2;
    ball.y = SCREEN_HEIGHT / 2;
    sprite_set_x(ball.ball_sprite, SCREEN_WIDTH  / 2);
    sprite_set_y(ball.ball_sprite, SCREEN_HEIGHT / 2);
    sprite_set_dx(ball.ball_sprite, 0);
    sprite_set_dy(ball.ball_sprite, 0);
    delay(1000);
    sprite_set_dx(ball.ball_sprite, x);
    sprite_set_dy(ball.ball_sprite, y);
}

int main()
{
    open_window("Homemade Pong", SCREEN_WIDTH, SCREEN_HEIGHT);

    load_resources();

    int y = SCREEN_HEIGHT/2;
    int start_dx = 6;
    int start_dy = 2;
    float dx = start_dx;
    float dy = start_dy;

    paddle_data player_1;
    paddle_data player_2;

    ball_data ball;
    ball = new_ball();

    player_1 = new_paddle(20 , y);
    player_2 = new_paddle(770, y);

    int score_1 = 0;
    int score_2 = 0;

    sprite_set_dx(ball.ball_sprite, dx);
    sprite_set_dy(ball.ball_sprite, dy);

    while( !quit_requested() )
    {
        process_events();

        clear_screen(background);

        if(key_typed(SPACE_KEY)) background = random_color();

        draw_paddle(player_1);
        draw_paddle(player_2);
        draw_ball(ball);

        update_paddle(player_1);
        update_paddle(player_2);
        update_ball(ball);

        controls_p1(player_1);
        controls_p2(player_2);

        draw_text(to_string(score_1), COLOR_ANTIQUE_WHITE, "roboto", 40, (SCREEN_WIDTH / 2) - 50, 20);
        draw_text(to_string(score_2), COLOR_ANTIQUE_WHITE, "roboto", 40, (SCREEN_WIDTH / 2) + 50, 20);

        if(sprite_x(ball.ball_sprite) <= 0)
        { 
            score_2 += 1;
            score(ball, start_dx, start_dy);
            dx = start_dx;
            dy = start_dy;
        }
        if(sprite_x(ball.ball_sprite) >= SCREEN_WIDTH)
        {
            score_1 += 1;
            score(ball, start_dx, start_dy);
            dx = start_dx;
            dy = start_dy;
        }

        if(sprite_y(ball.ball_sprite) <= 0 || sprite_y(ball.ball_sprite) >= SCREEN_HEIGHT)
        {
            dy = -dy;
            sprite_set_dy(ball.ball_sprite, dy);
        }

        if(sprite_collision(ball.ball_sprite, player_1.paddle_sprite) || sprite_collision(ball.ball_sprite, player_2.paddle_sprite))
        {
            play_sound_effect("pop");
            if (dx > 0) dx = dx + 0.5;
            else dx = dx - 0.5;
            dx = -dx;
            sprite_set_dx(ball.ball_sprite, dx);
            
            if(sprite_y(ball.ball_sprite) < SCREEN_HEIGHT/2)
            {
                dy = -dy;
                sprite_set_dy(ball.ball_sprite, dy);
            }
        }

        // write_line("ball x: "+to_string(ball.x));
        // write_line("player_2 y: "+to_string(player_2.y));
        // write_line("dx : "+ to_string(dx));

        refresh_screen(60);
    }

    return 0;
}

