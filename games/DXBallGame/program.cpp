#include "splashkit.h"

const int BLOCKS_IN_LEVEL1 = 110; // Number of blocks to be spawned in level 1
const double BLOCK_WIDTH = 60; // Block width
const double BLOCK_HEIGHT = 20; // Block height
const double BALL_RADIUS = 10; // Radius of ball
const double BALL_SPEED = 2; // Speed of ball movement
const double PADDLE_Y = 550; // Location of paddle on the y axis
const double PADDLE_HEIGHT = 5; // Height of the paddle
const double PADDLE_LENGTH = 100; // Length of the paddle

struct block_data
{
    double x; // Location on the x axis
    double y; // Location on the y axis
};

struct ball_data
{
    double x; // Location on the x axis
    double y; // Location on the y axis
    bool up; // Direction vertically, either up or down
    bool right; // Direction horizontally, either right or left
};

void draw_blocks(block_data blocks[], int remaining_blocks)
{
    // Draw blocks to the screen
    for (int i = 0; i < remaining_blocks; i++)
    {
        fill_rectangle(COLOR_BLUE, blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT);
        draw_rectangle(COLOR_BLACK, blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT);
    }
}

int main()
{
    open_window("DX Ball Game", 800, 600);
    window_toggle_border("DX Ball Game");

    // Spawn blocks
    int remaining_blocks = BLOCKS_IN_LEVEL1; // Number of blocks remaining
    block_data blocks[BLOCKS_IN_LEVEL1]; // Array containing blocks to be destroyed
    int index = 0;
    for (int i = 0; i < 5; i++) // draw 10 layers
    {
        for (int j = 0; j < 10; j++) // draw 10 blocks
        {
            block_data block; 
            block.x = 100 + j * BLOCK_WIDTH;
            block.y = 300 - i * 2 * BLOCK_HEIGHT;
            blocks[index] = block;
            index++;
        }
        for (int j = 0; j < 12; j++) // draw 12 blocks
        {
            block_data block;
            block.x = 40 + j * BLOCK_WIDTH;
            block.y = 300 - BLOCK_HEIGHT - i * 2 * BLOCK_HEIGHT;
            blocks[index] = block;
            index++;
        }
    }
    // Spawn ball at starting location
    ball_data ball;
    ball.x = screen_width()/2;
    ball.y = 500;
    ball.up = true;
    ball.right = true;

    // Paddle starting location at the x axis
    double paddle_x = (screen_width() - PADDLE_LENGTH) / 2;

    // Draw the environment
    clear_screen(COLOR_BLACK);
    draw_blocks(blocks, remaining_blocks); // Draw blocks
    fill_circle(COLOR_WHITE, ball.x, ball.y, BALL_RADIUS); // Draw ball
    fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle

    while(!key_down(ESCAPE_KEY))
    {
        // Player controls

        // Change ball movement
        // Bounce off walls
        if (ball.x - BALL_RADIUS <= 0) ball.right = true; // Bounce off left wall
        if (ball.x + BALL_RADIUS >= screen_width()) ball.right = false; // Bounce off right wall
        if (ball.y - BALL_RADIUS <= 0) ball.up = false; // Bounce off upper wall
        if (ball.y + BALL_RADIUS >= screen_height()) ball.up = true; // Bounce off bottom wall (to be removed)
        // Bounce off paddle
        if (ball.y + BALL_RADIUS == PADDLE_Y and (ball.x + BALL_RADIUS >= paddle_x and ball.x - BALL_RADIUS <= paddle_x + PADDLE_LENGTH))
            ball.up = true;
        // Bounce off blocks
        for (int i = 0; i < remaining_blocks; i++)
        {
            if (ball.y - BALL_RADIUS == blocks[i].y + BLOCK_HEIGHT and (ball.x >= blocks[i].x and ball.x <= blocks[i].x + BLOCK_WIDTH)) 
                ball.up = false; // Bounce off bottom of block
            if (ball.y + BALL_RADIUS == blocks[i].y and (ball.x >= blocks[i].x and ball.x <= blocks[i].x + BLOCK_WIDTH)) 
                ball.up = true; // Bounce off top of block
            if (ball.x - BALL_RADIUS == blocks[i].x + BLOCK_WIDTH and (ball.y >= blocks[i].y and ball.y <= blocks[i].y + BLOCK_HEIGHT)) 
                ball.right = true; // Bounce off right of block
            if (ball.x + BALL_RADIUS == blocks[i].x and (ball.y >= blocks[i].y and ball.y <= blocks[i].y + BLOCK_HEIGHT)) 
                ball.right = false; // Bounce off left of block
        }

        // Update ball locations
        if (ball.up) ball.y -= 1 * BALL_SPEED;
        else ball.y += 1 * BALL_SPEED;
        if (ball.right) ball.x += 1 * BALL_SPEED;
        else ball.x -= 1 * BALL_SPEED;
        
        // Redraw everything
        clear_screen(COLOR_BLACK);
        draw_blocks(blocks, remaining_blocks); // Draw blocks
        fill_circle(COLOR_WHITE, ball.x, ball.y, BALL_RADIUS); // Draw ball
        fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle

        refresh_screen(60);
    }
    return 0;
}

