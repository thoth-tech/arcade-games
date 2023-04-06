#include "splashkit.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
/*
 * Todo:
 *
 * add more levels and ability to start again after losing
 * maybe add a game data struct to store game state info
 */

const int BLOCKS_IN_LEVEL1 = 32; // Number of blocks to be spawned in level 1
const int BLOCKS_IN_LEVEL2 = 35;
const int BLOCKS_IN_LEVEL3 = 44;
const int BLOCKS_IN_LEVEL4 = 110;
const int BLOCKS_IN_LEVEL5 = 120;

const double BLOCK_WIDTH = 60; // Block width
const double BLOCK_HEIGHT = 20; // Block height
const double BALL_RADIUS = 8; // Radius of ball
const double BALL_SLOW_SPEED = 2; // Speed of a newly created ball before being bounced
const double BALL_SPEED = 4; // Speed of ball movement
const double PADDLE_SPEED = 8; //Speed of paddle movement
const double PADDLE_Y = 550; // Location of paddle on the y axis
const double PADDLE_HEIGHT = 5; // Height of the paddle
const double PADDLE_LENGTH = 100; // Length of the paddle
const double MULTIPLIER_DURATION = 10; // Duration in seconds of the score multiplier powerup


enum block_kind
{
    SINGLE_HIT, // Bricks take one hit to be destroyed
    DOUBLE_HIT, // Bricks take two hits to be destroyed
    HIDDEN,     // Bricks hidden from the player before being hit, then take one more hit to be destroyed
};

enum powerups
{
    NO_POWERUP,     // No powerup
	MULTI_BALL,		// Spawns another ball
	SCORE_MULTIPLY,	// Gives a temporary score multiplier
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
    double speed; // Speed of the ball's movement
};

void end_level(bool successful)
{
	if (successful)
	{
		draw_text("You win! Press r to play again", COLOR_WHITE, font_named("default"), 30, 100, 300);
	}
	else
	{
		draw_text("You lose, press r to try again", COLOR_WHITE, font_named("default"), 30, 100, 300);
	}
}

block_data create_block(double x, double y, block_kind kind, powerups powerup)
{
    block_data block;
    block.x = x;
    block.y = y;
	block.kind = kind;
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
    switch (powerup) //for now I'm using colours to distinguish powerup blocks, but this might not be ideal as it overrides the type colour
    {
        case MULTI_BALL:
            block.color = COLOR_PURPLE;
            break;
        case SCORE_MULTIPLY:
            block.color = COLOR_RED;
            break;
        default:
            break;
    }
    block.powerup = powerup;
    return block;
}

ball_data create_ball(double x, double y, bool up, bool right, double speed)
{
	ball_data ball;
    ball.x = x;
    ball.y = y;
    ball.up = up;
    ball.right = right;
	return ball;
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

void break_block(block_data blocks[], int block_index, int &score, int &remaining_blocks, int multiplier)
{
	blocks[block_index].hitpoint--;
	score += multiplier;
    if(blocks[block_index].hitpoint == 0)
    {
        remaining_blocks--;
    }
    else if (blocks[block_index].kind == DOUBLE_HIT)
    {
        blocks[block_index].color = COLOR_BEIGE; // Change color of bricks hit but not destroyed yet
    }	
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
        block = create_block(220 + i * BLOCK_WIDTH, 320, SINGLE_HIT, MULTI_BALL);
        blocks[index] = block;
        index++;
        block = create_block(220 + i * BLOCK_WIDTH, 280, SINGLE_HIT, SCORE_MULTIPLY);
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
    double timer = 0;
    int score_multiplier = 1;

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
	
	vector<ball_data> current_balls;
    // Spawn first ball at starting location
	current_balls.push_back(create_ball(screen_width()/2, 500, true, true, BALL_SPEED));

    // Paddle starting location at the x axis
    double paddle_x = (screen_width() - PADDLE_LENGTH) / 2;

    // Draw the environment
    clear_screen(COLOR_BLACK);
    draw_blocks(blocks, blocks_in_level); // Draw blocks
    fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle

    while(!key_down(ESCAPE_KEY))
    {
        if (timer > 0) timer -= (1.0 / 60.0); //count down 1/60 seconds every frame if the timer is in use
        else if (timer < 0) timer = 0;
        else score_multiplier = 1;
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
            // Reset score multiplier
            score_multiplier = 1;
            timer = 0;
            
            // Spawn ball at starting location
			current_balls.clear();
			current_balls.push_back(create_ball(screen_width()/2, 500, true, true, BALL_SPEED));

            // Paddle starting location at the x axis
            paddle_x = (screen_width() - PADDLE_LENGTH) / 2;

            next_level = false;
        }

		process_events(); //check keyboard state
		
        // Player controls
		if (key_down(LEFT_KEY) and paddle_x > 10) paddle_x -= PADDLE_SPEED; //moving left
		if (key_down(RIGHT_KEY) and paddle_x < screen_width() - PADDLE_LENGTH - 10) paddle_x += PADDLE_SPEED; //moving right
		
		for(int i = 0; i < current_balls.size(); i++)
		{
			// Ball collision
			// Bounce off walls
			if (current_balls[i].x - BALL_RADIUS <= 0) current_balls[i].right = true; // Bounce off left wall
			if (current_balls[i].x + BALL_RADIUS >= screen_width()) current_balls[i].right = false; // Bounce off right wall
			if (current_balls[i].y - BALL_RADIUS <= 0) current_balls[i].up = false; // Bounce off upper wall
			if (current_balls[i].y + BALL_RADIUS >= screen_height() and !game_over) // Hit bottom of the screen
			{
				//remove ball
				current_balls[i] = current_balls[current_balls.size()-1];
				current_balls.pop_back();
				
				if (current_balls.size() == 0) 
				{
					//lose the game if there are no more balls remaining
					game_won = false;
					game_over = true;
				}
			}
			// Bounce off paddle
            if ((PADDLE_Y <= current_balls[i].y + BALL_RADIUS and current_balls[i].y + BALL_RADIUS <= PADDLE_Y + BALL_SPEED) and (current_balls[i].x + BALL_RADIUS >= paddle_x and current_balls[i].x - BALL_RADIUS <= paddle_x + PADDLE_LENGTH))
            {
                current_balls[i].up = true;
                if (current_balls[i].speed == BALL_SLOW_SPEED) current_balls[i].speed = BALL_SPEED; //accelerate slowed balls
            }
			// Bounce off blocks
			for (int j = 0; j < blocks_in_level; j++)
			{
                bool collision = false;
				if (blocks[j].hitpoint > 0)
				{
					// Vertically
					if (current_balls[i].x >= blocks[j].x and current_balls[i].x <= blocks[j].x + BLOCK_WIDTH)
					{
						if (blocks[j].y + BLOCK_HEIGHT - BALL_SPEED <= current_balls[i].y - BALL_RADIUS and current_balls[i].y - BALL_RADIUS <= blocks[j].y + BLOCK_HEIGHT) // Bottom of block
						{
							current_balls[i].up = false;
							break_block(blocks, j, score, remaining_blocks, score_multiplier);
                            collision = true;
						}
						if (blocks[j].y <= current_balls[i].y + BALL_RADIUS and current_balls[i].y + BALL_RADIUS <= blocks[j].y + BALL_SPEED) // Top of block
						{
							current_balls[i].up = true;
							break_block(blocks, j, score, remaining_blocks, score_multiplier);
                            collision = true;
						}
					}
					// Horizontally
					if (current_balls[i].y + BALL_RADIUS >= blocks[j].y and current_balls[i].y - BALL_RADIUS < blocks[j].y + BLOCK_HEIGHT)
					{
						if (blocks[j].x + BLOCK_WIDTH <= current_balls[i].x - BALL_RADIUS - BALL_SPEED and current_balls[i].x - BALL_RADIUS <= blocks[j].x + BLOCK_WIDTH) // Right of block
						{
							current_balls[i].right = true;
							break_block(blocks, j, score, remaining_blocks, score_multiplier);
                            collision = true;
						}
						if (blocks[j].x <= current_balls[i].x + BALL_RADIUS and current_balls[i].x + BALL_RADIUS <= blocks[j].x + BALL_SPEED) // Left of block
						{
							current_balls[i].right = false;
							break_block(blocks, j, score, remaining_blocks, score_multiplier);
                            collision = true;
						}
					}
                    if (collision) // if the ball collided with any side of a block
                    {
                        //apply multi ball powerup
                        if (blocks[j].powerup == MULTI_BALL) current_balls.push_back(create_ball(blocks[j].x, blocks[j].y, current_balls[i].up, current_balls[i].right, BALL_SLOW_SPEED)); //slow new balls to make them easier to catch
                        //apply score multiplier powerup
                        else if (blocks[j].powerup == SCORE_MULTIPLY)
                        {
                            timer = MULTIPLIER_DURATION; // set timer for MULTIPLIER_DURATION seconds
                            score_multiplier++;
                        }
                    }
				}
			}

			// Update ball locations
			if (current_balls[i].up) current_balls[i].y -= 1 * BALL_SPEED;
			else current_balls[i].y += 1 * BALL_SPEED;
			if (current_balls[i].right) current_balls[i].x += 1 * BALL_SPEED;
			else current_balls[i].x -= 1 * BALL_SPEED;
		}
		
		//win level if level is completed
		if (remaining_blocks == 0)
		{
			next_level = true;
            current_level++;
		}
        
        // Redraw everything
		clear_screen(COLOR_BLACK);
		draw_blocks(blocks, blocks_in_level); // Draw blocks
		for(int i = 0; i < current_balls.size(); i++)
		{
			fill_circle(COLOR_WHITE, current_balls[i].x, current_balls[i].y, BALL_RADIUS); // Draw ball
		}
		fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle
		draw_text("SCORE: " + to_string(score) + " MULTIPLIER: x" + to_string(score_multiplier) + " " + to_string(timer), COLOR_WHITE, font_named("default"), 20, 20, 20); // Draw score
		
		//draw win/lose messages when level ends
		if (game_over)
		{
			end_level(game_won);

            if (key_typed(R_KEY)) //restart the game, can be moved to a function if we make a game data struct
            {
                score = 0;
                game_over = false;
                game_won = false;
                current_level = 1;
                next_level = false;
                timer = 0;
                score_multiplier = 1;

                // Spawn blocks
                blocks = spawn_blocks_level1(remaining_blocks);
                blocks_in_level = BLOCKS_IN_LEVEL1;

                // Spawn first ball at starting location
                current_balls.push_back(create_ball(screen_width() / 2, 500, true, true, BALL_SPEED));

                // Paddle starting location at the x axis
                paddle_x = (screen_width() - PADDLE_LENGTH) / 2;

                // Draw the environment
                clear_screen(COLOR_BLACK);
                draw_blocks(blocks, blocks_in_level); // Draw blocks
                fill_rectangle(COLOR_WHITE, paddle_x, PADDLE_Y, PADDLE_LENGTH, PADDLE_HEIGHT); // Draw paddle
            }
		}

        // Shortcut button to change level for development purpose
        //if (key_typed(NUM_1_KEY)) { next_level = true; current_level++; }

        refresh_screen(60);
    }
    return 0;
}

