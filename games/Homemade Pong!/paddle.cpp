#include "paddle.h"
#include "splashkit.h"

paddle_data new_paddle(int x, int y)
{
    paddle_data result;
    bitmap paddle = load_bitmap("paddle", "paddle.png");
    result.paddle_sprite = create_sprite(paddle);

    result.x = x;
    result.y = y;
    result.w = 10;
    result.l = 100;
    result.color = COLOR_WHITE;

    sprite_set_x(result.paddle_sprite, result.x);
    sprite_set_y(result.paddle_sprite, result.y);

    return result;
};

void draw_paddle(const paddle_data &paddle)
{
    draw_sprite(paddle.paddle_sprite);
}

void update_paddle(paddle_data &paddle)
{
    update_sprite(paddle.paddle_sprite);
}

void controls_p1(paddle_data &paddle)
{
    if(key_typed(R_KEY)) 
    {
        paddle.y = screen_height() / 2 - paddle.l/2;
        paddle.y = screen_height() / 2 - paddle.l/2;
        sprite_set_y(paddle.paddle_sprite, paddle.y);
    }

    if(key_typed(C_KEY)) paddle.color = random_color();

    if(key_down(W_KEY))
    {
        paddle.y = paddle.y - 7;
        sprite_set_y(paddle.paddle_sprite, paddle.y);
    }

    if(key_down(S_KEY)) 
    {
        paddle.y = paddle.y + 7;
        sprite_set_y(paddle.paddle_sprite, paddle.y);
    }

    if( paddle.y + LENGTH >= screen_height()) 
    {
        paddle.y = screen_height() - LENGTH;
        sprite_set_y(paddle.paddle_sprite, paddle.y);
    }
    if( paddle.y <= 0 ) 
    {
        paddle.y = 0;
        sprite_set_y(paddle.paddle_sprite, paddle.y);
    }

}

void controls_p2(paddle_data &paddle)
{
        if(key_typed(R_KEY)) 
        {
            paddle.y = screen_height() / 2 - paddle.l/2;
            paddle.y = screen_height() / 2 - paddle.l/2;
            sprite_set_y(paddle.paddle_sprite, paddle.y);
        }

        if(key_typed(RIGHT_CTRL_KEY)) paddle.color = random_color();

        if(key_down(UP_KEY)) 
        {
            paddle.y = paddle.y - 7;
            sprite_set_y(paddle.paddle_sprite, paddle.y);
        }

        if(key_down(DOWN_KEY)) 
        {
            paddle.y = paddle.y + 7;
            sprite_set_y(paddle.paddle_sprite, paddle.y);
        }

        if( paddle.y + LENGTH >= screen_height()) 
        {
            paddle.y = screen_height() - LENGTH;
            sprite_set_y(paddle.paddle_sprite, paddle.y);
        }

        if( paddle.y <= 0 ) 
        {
            paddle.y = 0;
            sprite_set_y(paddle.paddle_sprite, paddle.y);
        }
}


