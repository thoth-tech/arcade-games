#include "splashkit.h"
using namespace std;
/*
 * Todo:
 *
 * fix diagonal collisions (if the ball collides with a block diagonally it seems to pass through the block until it hits the center)
 * add more levels and ability to start again after losing
 * maybe add a game data struct to store game state info
 */

const int BLOCKS_IN_LEVEL1 = 110; // Number of blocks to be spawned in level 1
const double BLOCK_WIDTH = 60; // Block width
const double BLOCK_HEIGHT = 20; // Block height
const double BALL_RADIUS = 10; // Radius of ball
const double BALL_SPEED = 2; // Speed of ball movement
const double PADDLE_SPEED = 5; //Speed of paddle movement
const double PADDLE_Y = 550; // Location of paddle on the y axis
const double PADDLE_HEIGHT = 5; // Height of the paddle
const double PADDLE_LENGTH = 100; // Length of the paddle

struct block_data
{
    double x; // Location on the x axis
    double y; // Location on the y axis
	bool broken = false; //whether or not the ball has hit the brick
};

struct ball_data
{
    double x; // Location on the x axis
    double y; // Location on the y axis
    bool up; // Direction vertically, either up or down
    bool right; // Direction horizontally, either right or left
};

void end_level(bool successful)
{
	if (successful)
	{
		draw_text("You win!", COLOR_WHITE, font_named("default"), 50, 250, 300);
		//progress to next level
	}
	else
	{
		draw_text("You lose, try again?", COLOR_WHITE, font_named("default"), 50, 100, 300);
		//restart level
	}
}

void draw_blocks(block_data blocks[])
{
    // Draw blocks to the screen
    for (int i = 0; i < BLOCKS_IN_LEVEL1; i++)
    {
		if (!blocks[i].broken) //broken blocks aren't drawn
		{
			fill_rectangle(COLOR_BLUE, blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT);
			draw_rectangle(COLOR_BLACK, blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT);
		}
    }
}

void break_block(block_data blocks[], int block_index, int &score, int &remaining_blocks)
{
	blocks[block_index].broken = true;
	score++;
	remaining_blocks--;
}

int main()
{
	load_resource_bundle("game_bundle", "bundle.txt");
    open_window("DX Ball Game", 800, 600);
    window_toggle_border("DX Ball Game");
	
    // Spawn blocks
    int remaining_blocks = BLOCKS_IN_LEVEL1; // Number of blocks remaining
	int score = 0;
	bool game_over = false;
	bool game_won = false;
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
	
	// spawn only one block for testing winning a level
	/*
	block_data block; 
    block.x = 600;
    block.y = 300;
    blocks[index] = block;
	*/
	
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
    draw_blocks(blocks); // Draw blocks
    fill_circle(COLOR_WHITE, ball.x, ball.y, BALL_RADIUS); // Draw ball
    fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle

    while(!key_down(ESCAPE_KEY))
    {
		process_events(); //check keyboard state
		
        // Player controls
		if (key_down(LEFT_KEY) and paddle_x > 10) paddle_x -= PADDLE_SPEED; //moving left
		if (key_down(RIGHT_KEY) and paddle_x < screen_width() - PADDLE_LENGTH - 10) paddle_x += PADDLE_SPEED; //moving right
		
        // Ball collision
        // Bounce off walls
        if (ball.x - BALL_RADIUS <= 0) ball.right = true; // Bounce off left wall
        if (ball.x + BALL_RADIUS >= screen_width()) ball.right = false; // Bounce off right wall
        if (ball.y - BALL_RADIUS <= 0) ball.up = false; // Bounce off upper wall
        if (ball.y + BALL_RADIUS >= screen_height() and !game_over) // Hit bottom of the screen
		{
			//lose the game
			game_won = false;
			game_over = true;
		}
        // Bounce off paddle
        if (ball.y + BALL_RADIUS == PADDLE_Y and (ball.x + BALL_RADIUS >= paddle_x and ball.x - BALL_RADIUS <= paddle_x + PADDLE_LENGTH))
            ball.up = true;
        // Bounce off blocks
        for (int i = 0; i < BLOCKS_IN_LEVEL1; i++)
        {
            if (!blocks[i].broken)
            {
                // Vertically
                if (ball.x >= blocks[i].x and ball.x <= blocks[i].x + BLOCK_WIDTH)
                {
                    if (ball.y - BALL_RADIUS == blocks[i].y + BLOCK_HEIGHT) // Bottom of block
                    {
                        ball.up = false;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                    if (ball.y + BALL_RADIUS == blocks[i].y) // Top of block
                    {
                        ball.up = true;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                }
                // Horizontally
                if (ball.y + BALL_RADIUS >= blocks[i].y and ball.y - BALL_RADIUS < blocks[i].y + BLOCK_HEIGHT)
                {
                    if (ball.x - BALL_RADIUS == blocks[i].x + BLOCK_WIDTH) // Right of block
                    {
                        ball.right = true;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                    if (ball.x + BALL_RADIUS == blocks[i].x) // Left of block
                    {
                        ball.right = false;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                }
            }
        }

        // Update ball locations
        if (ball.up) ball.y -= 1 * BALL_SPEED;
        else ball.y += 1 * BALL_SPEED;
        if (ball.right) ball.x += 1 * BALL_SPEED;
        else ball.x -= 1 * BALL_SPEED;
		
		//win level if level is completed
		if (remaining_blocks == 0)
		{
			game_won = true;
			game_over = true;
		}
        
        // Redraw everything
		clear_screen(COLOR_BLACK);
		draw_blocks(blocks); // Draw blocks
		if (!game_over) fill_circle(COLOR_WHITE, ball.x, ball.y, BALL_RADIUS); // Draw ball
		fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle
		draw_text("SCORE: " + to_string(score), COLOR_WHITE, font_named("default"), 20, 20, 20); // Draw score
		
		//draw win/lose messages when level ends
		if (game_over)
		{
			end_level(game_won);
		}

        refresh_screen(60);
    }
    return 0;
}

