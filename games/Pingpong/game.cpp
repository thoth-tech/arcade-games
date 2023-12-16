#include "splashkit.h"
#include "game.h"
#include <cmath>
#include <vector>
using namespace std;
using std::vector;
#define GAME_FINISH_SCORE 3
#define PLAYER_MOVE_SPEED 10
#define BALL_MIN_ANGLE 60
#define BALL_MAX_ANGLE 300
// Function to set up the game
void setup(game_data &game)
{
    // Draw the background
    if (game.type == AIRHOCKEY)
    {
        draw_bitmap("background2", 0, 0, option_to_screen());
    }
    else
    {
        draw_bitmap("background", 0, 0, option_to_screen());
    }

    // Draw the obstacles
    for (int i = 0; i < game.obstacles.size(); i++)
    {
        draw_obstacle(game.obstacles[i]);
    }
    
    
}

// Function to create a new game
game_data new_game()
{
    game_data game;

    // Initialize the game data
    game.player1 = new_player(RED_PLAYER, true);
    game.player2 = new_player(BLUE_PLAYER, false);
    game.ball = new_ball(SOCCER);
    game.screen = 1;
    game.intro_played = false;
    game.type = AIRHOCKEY;
   
    game.time = create_timer("game");

    // Create the obstacles
    obstacle_data Up = new_obstacle(HORIZONTAL, 0, 0);
    obstacle_data Down = new_obstacle(HORIZONTAL, 0, 680);
    obstacle_data left1 = new_obstacle(VERTICAL, 0, 40);
    obstacle_data left2 = new_obstacle(VERTICAL, 0, 444);
    obstacle_data right1 = new_obstacle(VERTICAL, 1240, 40);
    obstacle_data right2 = new_obstacle(VERTICAL, 1240, 444);
    game.obstacles.push_back(Up);
    game.obstacles.push_back(Down);
    game.obstacles.push_back(left1);
    game.obstacles.push_back(left2);
    game.obstacles.push_back(right1);
    game.obstacles.push_back(right2);

    // Delay for 1 second
    delay(1000);

    return game;
}

bool hasCollided = false;
void bounce_vertical(game_data &game)
{
    // Reverse the angle
    // UP -> 360 - ANGLE
    game.ball.yspeed = -game.ball.yspeed;
    sprite_set_dy(game.ball.ball_sprite, game.ball.yspeed);
    play_sound_effect("hit", 1, 0.05);
    write_line("vertical collision");
}
void bounce_horizontal(game_data &game)
{
    // LEFT -> 180 - ANGLE
    // game.ball.angle = 180 - game.ball.angle;
    // sprite_set_rotation(game.ball.ball_sprite, game.ball.angle);
    game.ball.xspeed = -game.ball.xspeed;
    sprite_set_dx(game.ball.ball_sprite, game.ball.xspeed);
    play_sound_effect("hit", 1, 0.05);
    write_line("horizontal collision");
}

// Function to handle ball collisions
void collisions_ball(game_data &game)
{
    // Flag to track if a collision has occurred
    // Check for collision with the left obstacle or player 1
    if (sprite_collision(game.ball.ball_sprite, game.player1.player_sprite) && !hasCollided)
    {
        hasCollided = true;
        // Check if the ball is colliding with the top or bottom of player 1
        float ballTop = sprite_y(game.ball.ball_sprite);
        float ballBottom = ballTop + sprite_height(game.ball.ball_sprite);

        float player1Top = sprite_y(game.player1.player_sprite);
        float player1Bottom = player1Top + sprite_height(game.player1.player_sprite);

        if (ballBottom <= player1Top || ballTop >= player1Bottom)
        {
            // Reverse the vertical component
            bounce_vertical(game);
            hasCollided = false; // Set the flag to true
        }
        else
        {
            // LEFT -> 180 - ANGLE
            bounce_horizontal(game);
            game.ball.xspeed += 0.5;
            sprite_set_dx(game.ball.ball_sprite, game.ball.xspeed);
            hasCollided = false; // Set the flag to true
        }
    }
    if (sprite_collision(game.ball.ball_sprite, game.player2.player_sprite) && !hasCollided)
    {
        hasCollided = true;
        // Check if the ball is colliding with the top or bottom of player 2
        float ballTop = sprite_y(game.ball.ball_sprite);
        float ballBottom = ballTop + sprite_height(game.ball.ball_sprite);

        float player2Top = sprite_y(game.player2.player_sprite);
        float player2Bottom = player2Top + sprite_height(game.player2.player_sprite);

        if ((ballBottom <= player2Top || ballTop >= player2Bottom))
        {
            // Reverse the vertical component
            bounce_vertical(game);
            hasCollided = false; // Set the flag to true
        }
        else
        {
            // LEFT -> 180 - ANGLE

            bounce_horizontal(game);
            game.ball.xspeed += 0.5;
            sprite_set_dx(game.ball.ball_sprite, game.ball.xspeed);
            hasCollided = false; // Set the flag to true
        }
    }

    // Check for collision with the top obstacle
    if (sprite_collision(game.ball.ball_sprite, game.obstacles[0].obstacle_sprite) && !hasCollided)
    {
        bounce_vertical(game);
        hasCollided = false; // Set the flag to true
    }

    // Check for collision with the bottom obstacle
    if (sprite_collision(game.ball.ball_sprite, game.obstacles[1].obstacle_sprite) && !hasCollided)
    {
        // DOWN -> 360 - ANGLE
        bounce_vertical(game);
        hasCollided = false; // Set the flag to true
    }
    if ((sprite_collision(game.ball.ball_sprite, game.obstacles[2].obstacle_sprite) || sprite_collision(game.ball.ball_sprite, game.obstacles[3].obstacle_sprite)) && !hasCollided)
    {

        hasCollided = true;

        float obstacle3Top = sprite_y(game.obstacles[3].obstacle_sprite);
        float obstacle2Bottom = sprite_y(game.obstacles[2].obstacle_sprite) + sprite_height(game.obstacles[2].obstacle_sprite);
        if (game.ball.ball_top >= obstacle2Bottom && game.ball.ball_bottom <= obstacle3Top)
        {
            bounce_vertical(game);
            hasCollided = false; // Set the flag to true
        }
        else
        {
            bounce_horizontal(game);
            hasCollided = false; // Set the flag to true
        }
    }
    // Check for collision with the right obstacle or player 2
    if ((sprite_collision(game.ball.ball_sprite, game.obstacles[4].obstacle_sprite) || sprite_collision(game.ball.ball_sprite, game.obstacles[5].obstacle_sprite)) && !hasCollided)
    {
        hasCollided = true;
        float ballTop = sprite_y(game.ball.ball_sprite);
        float ballBottom = ballTop + sprite_height(game.ball.ball_sprite);

        float obstacle5Top = sprite_y(game.obstacles[5].obstacle_sprite);
        float obstacle4Bottom = sprite_y(game.obstacles[4].obstacle_sprite) + sprite_height(game.obstacles[4].obstacle_sprite);
        if ((ballTop >= obstacle4Bottom && ballBottom <= obstacle5Top) || (sprite_x(game.ball.ball_sprite) > sprite_x(game.obstacles[4].obstacle_sprite)) || ((sprite_x(game.ball.ball_sprite) > sprite_x(game.obstacles[5].obstacle_sprite))))
        {
            bounce_vertical(game);
            hasCollided = false; // Set the flag to true
        }
        else
        {
            bounce_horizontal(game);
            hasCollided = false; // Set the flag to true
        }
    }
    // Check for collision with the left obstacle or player 2
}

// Function to reset the ball's position and motion
void reset_ball(game_data &game)
{
    // Reset the ball's position to the center of the screen
    sprite_set_x(game.ball.ball_sprite, (screen_width() / 2) - 25);
    sprite_set_y(game.ball.ball_sprite, (screen_height() / 2) - 25);
    delay(1000);

    // Reset the ball's motion
    ball_motion(game.ball);
}

// Function to check if the ball is out of the map and update the scores
void check_ball_out_of_map(game_data &game)
{
    double ball_x = sprite_x(game.ball.ball_sprite);

    // Check if the ball is out of the left side of the map
    if (ball_x < 0)
    {
        // Player 2 scores
        play_sound_effect("score");
        game.player2.score++;
        reset_ball(game);
    }
    // Check if the ball is out of the right side of the map
    else if (ball_x > screen_width())
    {
        // Player 1 scores
        play_sound_effect("score");
        game.player1.score++;
        reset_ball(game);
    }

    
}

// Function to handle player controls
void controls(game_data &game)
{
    // Move player 1 up and down
    double player1_pos = sprite_y(game.player1.player_sprite);
    if (player1_pos < 40)
    {
        sprite_set_y(game.player1.player_sprite, 40);
    }
    else if (game.player1.player_bottom > 680)
    {
        sprite_set_y(game.player1.player_sprite, 680-sprite_height(game.player1.player_sprite));
    }
    else
    {
        if (key_down(W_KEY))
        {
            sprite_set_y(game.player1.player_sprite, player1_pos - PLAYER_MOVE_SPEED);
        }
        if (key_down(S_KEY))
        {
            sprite_set_y(game.player1.player_sprite, player1_pos + PLAYER_MOVE_SPEED);
        }
    }

    // Move player 2 up and down
    double player2_pos = sprite_y(game.player2.player_sprite);
    if (player2_pos < 40)
    {
        sprite_set_y(game.player2.player_sprite, 40);
    }
    else if (game.player2.player_bottom > 680)
    {
        sprite_set_y(game.player2.player_sprite, 680-sprite_height(game.player2.player_sprite));
    }
    else
    {
        if (key_down(UP_KEY))
        {
            sprite_set_y(game.player2.player_sprite, player2_pos - 10);
        }
        if (key_down(DOWN_KEY))
        {
            sprite_set_y(game.player2.player_sprite, player2_pos + 10);
        }
    }
    // Move player 1 left and right
    double player1_posx = sprite_x(game.player1.player_sprite);
    if (player1_posx < 100)
    {
        sprite_set_x(game.player1.player_sprite, 100);
    }
    else if (player1_posx > 550)
    {
        sprite_set_x(game.player1.player_sprite, 550);
    }
    else
    {
        if (key_down(A_KEY))
        {
            sprite_set_x(game.player1.player_sprite, player1_posx - 10);
        }
        if (key_down(D_KEY))
        {
            sprite_set_x(game.player1.player_sprite, player1_posx + 10);
        }
    }

    // Move player 2 left and right
    double player2_posx = sprite_x(game.player2.player_sprite);
    if (player2_posx < 690)
    {
        sprite_set_x(game.player2.player_sprite, 690);
    }
    else if (player2_posx > 1200)
    {
        sprite_set_x(game.player2.player_sprite, 1200);
    }
    else
    {
        if (key_down(LEFT_KEY))
        {
            sprite_set_x(game.player2.player_sprite, player2_posx - 10);
        }
        if (key_down(RIGHT_KEY))
        {
            sprite_set_x(game.player2.player_sprite, player2_posx + 10);
        }
    }
}
void draw_start(game_data &game)
{
    draw_bitmap("loaded", 0, 0, option_to_screen());
    if (!game.intro_played)
    {
        play_sound_effect("intro", 1, 0.3);
        game.intro_played = true;
    }
}

void draw_end(game_data &game)
{

    if (!game.ended)
    {
        play_sound_effect("outro", 1, 0.2);
        game.ended = true;
    }
    draw_bitmap("gameover", 0, 0);
    if (game.player1.score >= GAME_FINISH_SCORE)
    {
        draw_text("player 1 won", COLOR_WHITE, "font1", 80, 317, 380, option_to_screen());
    }
    if (game.player2.score >= GAME_FINISH_SCORE)
    {
        draw_text("Player 2 won", COLOR_WHITE, "font1", 80, 317, 380, option_to_screen());
    }
}
void update_game(game_data &game)
{
    if (game.screen == 4)
    {
        controls(game);
        update_player(game.player1);
        update_player(game.player2);
        update_ball(game.ball);
        collisions_ball(game);
        for (int i = 0; i < game.obstacles.size(); i++)
        {
            update_obstacle(game.obstacles[i]);
        }
        check_ball_out_of_map(game);
    }
    if (game.player1.score >= GAME_FINISH_SCORE || game.player2.score >= GAME_FINISH_SCORE)
    {
        // Check if player 1 or player 2 has won
        game.screen = 5;
    }
}
bool temp = false;
void draw_game(game_data &game)
{
    if (game.screen == 1)
    {
        stop_sound_effect("outro");
        draw_start(game);
        if (key_typed(RETURN_KEY))
        {
            game.screen++;
        }
    }
    else if (game.screen == 2)
    {
        // Draw the controls screen
        draw_bitmap("controls", 0, 0, option_to_screen());
        // Check if the return key is pressed to start the game
        if (key_typed(RETURN_KEY))
        {
            game.screen++;
        }
    }
    else if (game.screen == 3)
    {

        if (game.type == AIRHOCKEY)
        {
            draw_bitmap("pchoose", 0, 0, option_to_screen());
        }
        else
        {
            draw_bitmap("schoose", 0, 0, option_to_screen());
        }
        if (key_typed(LEFT_KEY) || key_typed(A_KEY))
        {
            draw_bitmap("pchoose", 0, 0, option_to_screen());
            game.type = AIRHOCKEY;
        }
        else if (key_typed(RIGHT_KEY) || key_typed(D_KEY))
        {
            draw_bitmap("schoose", 0, 0, option_to_screen());
            game.type = FOOTBALL;
        }
        else if (key_typed(RETURN_KEY))
        {

            if (game.type == FOOTBALL)
            {
                game.player1 = new_player(RED_PLAYER, true);
                game.player2 = new_player(BLUE_PLAYER, false);
                game.ball = new_ball(SOCCER);
            }
            else
            {
                game.player1 = new_player(PLAYER1, true);
                game.player2 = new_player(PLAYER2, false);
                game.ball = new_ball(COLORFUL);
            }
            game.screen++;
            ball_motion(game.ball);
        }
    }
    else if (game.screen == 4)
    {
        // Set up the game
        setup(game);
        // Draw the players and the ball
        draw_player(game.player1);
        draw_player(game.player2);
        draw_ball(game.ball);
        // Start the ball motion if the game has just started
        if (!game.started)
        {
            ball_motion(game.ball);
            start_timer("game");
        }
        game.started = true;
        // Draw the player names and scores
        if(game.type == AIRHOCKEY) {
        draw_text("Player 1", COLOR_RED, "font2", 40, 100, 0, option_to_screen());
        draw_text("Player 2", COLOR_BLUE, "font2", 40, 880, 0, option_to_screen());
        draw_text(to_string(game.player1.score), COLOR_RED, "font1", 60, 5, 200, option_to_screen());
        draw_text(to_string(game.player2.score), COLOR_BLUE, "font1", 60, 1245, 200, option_to_screen());
        draw_text("Score " + to_string(GAME_FINISH_SCORE) + " to win", COLOR_YELLOW, "font2", 30, 480, 680, option_to_screen());
           int times = timer_ticks("game")/1000;
        write_line(times);
        int minute = times /60;
        string mins = to_string(minute);
        if(minute<10) mins = "0"+mins;
        int seconds  = times %60;
        string secs = to_string(seconds);
        if(seconds <10) secs = "0"+secs;
         draw_text(mins + ":" + secs, COLOR_WHITE, "font1", 20, 627, 19, option_to_screen());
        
        }
        else {
            draw_bitmap("scoreboard", 520, 0);
        draw_text(to_string(game.player1.score), COLOR_RED, "font1", 60, 555, 63, option_to_screen());
        draw_text(to_string(game.player2.score), COLOR_BLUE, "font1", 60, 700, 63, option_to_screen());
       draw_text("Score " + to_string(GAME_FINISH_SCORE) + " to win", COLOR_YELLOW, "font2", 30, 480, 680, option_to_screen());
       int times = timer_ticks("game")/1000;
        write_line(times);
        int minute = times /60;
        string mins = to_string(minute);
        if(minute<10) mins = "0"+mins;
        int seconds  = times %60;
        string secs = to_string(seconds);
        if(seconds <10) secs = "0"+secs;
         draw_text(mins + ":" + secs, COLOR_WHITE, "font1", 20, 627, 19, option_to_screen());
        }
        if (key_down(ESCAPE_KEY))
        {
            game.player1.score = 3;
        }
    }
    else
    {
        stop_sound_effect("intro");
        draw_end(game);

        if (key_down(R_KEY))
        {
            game = new_game();
            reset_timer("game");
        }
    }
}
