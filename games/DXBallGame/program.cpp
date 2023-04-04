#include "splashkit.h"
#include <string>
#include <iostream>
using namespace std;
/*
 * Todo:
 *
 * add more levels and ability to start again after losing
 * maybe add a game data struct to store game state info
 * add powerups
 */

const int BLOCKS_IN_LEVEL1 = 32; // Number of blocks to be spawned in level 1
const int BLOCKS_IN_LEVEL2 = 35;
const int BLOCKS_IN_LEVEL3 = 44;
const int BLOCKS_IN_LEVEL4 = 110;
const int BLOCKS_IN_LEVEL5 = 120;

const double BLOCK_WIDTH = 60; // Block width
const double BLOCK_HEIGHT = 20; // Block height
const double BALL_RADIUS = 8; // Radius of ball
const double BALL_SPEED = 4; // Speed of ball movement
const double PADDLE_SPEED = 8; //Speed of paddle movement
const double PADDLE_Y = 550; // Location of paddle on the y axis
const double PADDLE_HEIGHT = 5; // Height of the paddle
const double PADDLE_LENGTH = 100; // Length of the paddle


enum block_kind
{
    SINGLE_HIT, // Bricks take one hit to be destroyed
    DOUBLE_HIT, // Bricks take two hits to be destroyed
    HIDDEN,     // Bricks hidden from the player before being hit, then take one more hit to be destroyed
};

enum powerups
{
    NO_POWERUP,     // No powerup
};

struct block_data
{
    double x; // Location on the x axis
    double y; // Location on the y axis
	//bool broken = false; //whether or not the ball has hit the brick
    block_kind kind; // Type of brick
    powerups powerup; // Powerup that appear after brick is destroyed
    int hitpoint; // How many hits left to be destroyed
    color color; // Color of block
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

block_data create_block(double x, double y, block_kind kind, powerups powerup)
{
    block_data block;
    block.x = x;
    block.y = y;
    switch (kind)
    {
        case SINGLE_HIT:
            block.hitpoint = 1;
            block.color = COLOR_BLUE;
            break;
        case DOUBLE_HIT:
            block.hitpoint = 2;
            block.color = COLOR_YELLOW;
            break;
        case HIDDEN:
            block.hitpoint = 2;
            block.color = COLOR_BLACK;
            break;
    }
    block.powerup = powerup;
    return block;
}

void draw_blocks(block_data blocks[], int blocks_in_level)
{
    // Draw blocks to the screen
    for (int i = 0; i < blocks_in_level; i++)
    {
		if (blocks[i].hitpoint > 0) //broken blocks aren't drawn
		{
			fill_rectangle(blocks[i].color, blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT);
			draw_rectangle(COLOR_BLACK, blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT);
		}
    }
}

void break_block(block_data blocks[], int block_index, int &score, int &remaining_blocks)
{
	blocks[block_index].hitpoint--;
	score++;
    if(blocks[block_index].hitpoint == 0)
    {
        remaining_blocks--;
    }
    else 
    {
        blocks[block_index].color = COLOR_BEIGE; // Change color of bricks hit but not destroyed yet
    }

    /*
        Changing color by checking 
        if (blocks[block_index].kind == DOUBLE_HIT)
        always returns false for some reason
    */
}

block_data * spawn_blocks_level1(int &remaining_blocks)
{
    remaining_blocks = BLOCKS_IN_LEVEL1; // Number of blocks remaining
    static block_data blocks[BLOCKS_IN_LEVEL1]; // Array containing blocks to be destroyed
    block_data block;
    int index = 0;

    for (int i = 0; i < 8; i++)
    {
        block = create_block(160 + i * BLOCK_WIDTH, 300, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 6; i++)
    {
        block = create_block(220 + i * BLOCK_WIDTH, 320, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(220 + i * BLOCK_WIDTH, 280, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 4; i++)
    {
        block = create_block(280 + i * BLOCK_WIDTH, 340, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(280 + i * BLOCK_WIDTH, 260, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 2; i++)
    {
        block = create_block(340 + i * BLOCK_WIDTH, 360, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(340 + i * BLOCK_WIDTH, 240, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }

    return blocks;
}
block_data * spawn_blocks_level2(int &remaining_blocks)
{
    static block_data blocks[BLOCKS_IN_LEVEL2];
    remaining_blocks = BLOCKS_IN_LEVEL2;
    int index = 0;
    block_data block;

    for (int i = 0; i < 7; i++)
    {
        block = create_block(40 + i * BLOCK_WIDTH, 300 - i * BLOCK_HEIGHT, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(100 + i * BLOCK_WIDTH, 300 - i  * BLOCK_HEIGHT, DOUBLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(280 + i * BLOCK_WIDTH, 300 - i * BLOCK_HEIGHT, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(340 + i * BLOCK_WIDTH, 300 - i  * BLOCK_HEIGHT, DOUBLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 3; i++)
    {
        block = create_block(520 + i * BLOCK_WIDTH, 300 - i * BLOCK_HEIGHT, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(580 + i * BLOCK_WIDTH, 300 - i  * BLOCK_HEIGHT, DOUBLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    block = create_block(700, 240, SINGLE_HIT, NO_POWERUP);
    blocks[index] = block;

    return blocks;
}
block_data * spawn_blocks_level3(int &remaining_blocks)
{
    static block_data blocks[BLOCKS_IN_LEVEL3];
    remaining_blocks = BLOCKS_IN_LEVEL3;
    int index = 0;
    block_data block;

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            block = create_block(160 + j * BLOCK_WIDTH, 300 - i * BLOCK_HEIGHT, SINGLE_HIT, NO_POWERUP);
            blocks[index] = block;
            index++;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        block = create_block(40 + i * BLOCK_WIDTH, 360, HIDDEN, NO_POWERUP);
        blocks[index] = block;
        index++;
    }

    return blocks;
}
block_data * spawn_blocks_level4(int &remaining_blocks)
{
    remaining_blocks = BLOCKS_IN_LEVEL4; // Number of blocks remaining
    static block_data blocks[BLOCKS_IN_LEVEL4]; // Array containing blocks to be destroyed
    int index = 0;
    block_data block;

    for (int i = 0; i < 5; i++) // draw 10 layers
    {
        for (int j = 0; j < 10; j++) // draw 10 blocks
        {
            double x = 100 + j * BLOCK_WIDTH;
            double y = 300 - i * 2 * BLOCK_HEIGHT;
            block_kind kind = DOUBLE_HIT;
            powerups powerup = NO_POWERUP;
            block = create_block(x, y, kind, powerup);
            blocks[index] = block;
            index++;
        }
        for (int j = 0; j < 12; j++) // draw 12 blocks
        {
            double x = 40 + j * BLOCK_WIDTH;
            double y = 300 - BLOCK_HEIGHT - i * 2 * BLOCK_HEIGHT;
            block_kind kind = SINGLE_HIT;
            powerups powerup = NO_POWERUP;
            block = create_block(x, y, kind, powerup);
            blocks[index] = block;
            index++;
        }
    }
    return blocks;
}
block_data * spawn_blocks_level5(int &remaining_blocks)
{
    static block_data blocks[BLOCKS_IN_LEVEL5];
    remaining_blocks = BLOCKS_IN_LEVEL5;
    block_data block;
    int index = 0;
    for(int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            block = create_block(40 + j * BLOCK_WIDTH, 300 - i * BLOCK_HEIGHT, static_cast<block_kind>(rand() % 3), NO_POWERUP);
            blocks[index] = block;
            index++;
        }
    }

    return blocks;
}

int main()
{
	load_resource_bundle("game_bundle", "bundle.txt");
    open_window("DX Ball Game", 800, 600);
    window_toggle_border("DX Ball Game");
	
    block_data *blocks;
    int remaining_blocks;
    int blocks_in_level;
    int score = 0;
    bool game_over = false;
    bool game_won = false;
    int current_level = 1;
    bool next_level = false;

    // Spawn blocks
    blocks = spawn_blocks_level1(remaining_blocks);
    blocks_in_level = BLOCKS_IN_LEVEL1;
	
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
    draw_blocks(blocks, blocks_in_level); // Draw blocks
    fill_circle(COLOR_WHITE, ball.x, ball.y, BALL_RADIUS); // Draw ball
    fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle

    while(!key_down(ESCAPE_KEY))
    {
        // Start level
        if (next_level)
        {
            switch (current_level)
            {
                case 2: 
                    blocks = spawn_blocks_level2(remaining_blocks);
                    blocks_in_level = BLOCKS_IN_LEVEL2;
                    break;
                case 3: 
                    blocks = spawn_blocks_level3(remaining_blocks);
                    blocks_in_level = BLOCKS_IN_LEVEL3;
                    break;
                case 4:
                    blocks = spawn_blocks_level4(remaining_blocks);
                    blocks_in_level = BLOCKS_IN_LEVEL4;
                    break;
                case 5:
                    blocks = spawn_blocks_level5(remaining_blocks);
                    blocks_in_level = BLOCKS_IN_LEVEL5;
                    break;
                default:
                    game_won = true;
                    game_over = true;
                    break;
            }
            
            // Spawn ball at starting location
            ball.x = screen_width()/2;
            ball.y = 500;
            ball.up = true;
            ball.right = true;

            // Paddle starting location at the x axis
            paddle_x = (screen_width() - PADDLE_LENGTH) / 2;

            next_level = false;
        }

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
        if ((PADDLE_Y <= ball.y + BALL_RADIUS and ball.y + BALL_RADIUS <= PADDLE_Y + BALL_SPEED) and (ball.x + BALL_RADIUS >= paddle_x and ball.x - BALL_RADIUS <= paddle_x + PADDLE_LENGTH))
            ball.up = true;
        // Bounce off blocks
        for (int i = 0; i < blocks_in_level; i++)
        {
            if (blocks[i].hitpoint > 0)
            {
                // Vertically
                if (ball.x >= blocks[i].x and ball.x <= blocks[i].x + BLOCK_WIDTH)
                {
                    if (blocks[i].y + BLOCK_HEIGHT - BALL_SPEED <= ball.y - BALL_RADIUS and ball.y - BALL_RADIUS <= blocks[i].y + BLOCK_HEIGHT) // Bottom of block
                    {
                        ball.up = false;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                    if (blocks[i].y <= ball.y + BALL_RADIUS and ball.y + BALL_RADIUS <= blocks[i].y + BALL_SPEED) // Top of block
                    {
                        ball.up = true;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                }
                // Horizontally
                if (ball.y + BALL_RADIUS >= blocks[i].y and ball.y - BALL_RADIUS < blocks[i].y + BLOCK_HEIGHT)
                {
                    if (blocks[i].x + BLOCK_WIDTH <= ball.x - BALL_RADIUS - BALL_SPEED and ball.x - BALL_RADIUS <= blocks[i].x + BLOCK_WIDTH) // Right of block
                    {
                        ball.right = true;
                        break_block(blocks, i, score, remaining_blocks);
                    }
                    if (blocks[i].x <= ball.x + BALL_RADIUS and ball.x + BALL_RADIUS <= blocks[i].x + BALL_SPEED) // Left of block
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
			next_level = true;
            current_level++;
		}
        
        // Redraw everything
		clear_screen(COLOR_BLACK);
		draw_blocks(blocks, blocks_in_level); // Draw blocks
		if (!game_over) fill_circle(COLOR_WHITE, ball.x, ball.y, BALL_RADIUS); // Draw ball
		fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle
		draw_text("SCORE: " + to_string(score), COLOR_WHITE, font_named("default"), 20, 20, 20); // Draw score
		
		//draw win/lose messages when level ends
		if (game_over)
		{
			end_level(game_won);
		}

        // Shortcut button to change level for development purpose
        //if (key_typed(NUM_1_KEY)) { next_level = true; current_level++; }

        refresh_screen(60);
    }
    return 0;
}

