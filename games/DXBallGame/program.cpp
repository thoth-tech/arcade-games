#include "splashkit.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
/*
 * Todo:
 *
 * apply score multiplier sprites
 * add powerups in other levels
 */

const int BLOCKS_IN_LEVEL1 = 32; // Number of blocks to be spawned in level 1
const int BLOCKS_IN_LEVEL2 = 35;
const int BLOCKS_IN_LEVEL3 = 44;
const int BLOCKS_IN_LEVEL4 = 120;

const double BLOCK_WIDTH = 60;         // Block width
const double BLOCK_HEIGHT = 20;        // Block height
const double BALL_RADIUS = 8;          // Radius of ball
const double BALL_SPEED = 4;           // Speed of ball movement
const double PADDLE_SPEED = 8;         // Speed of paddle movement
const double PADDLE_Y = 550;           // Location of paddle on the y axis
const double PADDLE_HEIGHT = 5;        // Height of the paddle
const double PADDLE_LENGTH = 100;      // Length of the paddle
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
    double x;               // Location on the x axis
    double y;               // Location on the y axis
    block_kind kind;        // Type of brick
    powerups powerup;       // Powerup that appear after brick is destroyed
    int hitpoint;           // How many hits left to be destroyed
    string block_bitmap;    // Bitmap used for the block
    string powerup_bitmap;  // Bitmap used to show powerup contained in the block
};
struct ball_data
{
    double x;   // Location on the x axis
    double y;   // Location on the y axis
    bool up;    // Direction vertically, either up or down
    bool right; // Direction horizontally, either right or left
};
struct powerup_drop_data
{
	double x;      // Location on the x axis
    double y;      // Location on the y axis
	powerups kind; // Type of powerup
};

struct
{
    block_data *blocks;         // Array of blocks that exist in the current level
    vector<powerup_drop_data> current_powerups; // Vector of powerups currently dropping
	vector<ball_data> current_balls;            // Vector of balls currently active
    int remaining_blocks;       // Blocks remaining to destroy to complete level
    int blocks_in_level;        // Initial number of blocks
    int score = 0;              // Current score (default starting score = 0)
    bool game_over = false;     // True if all levels complete or no more ball remaining
    bool game_won = false;      // True if all levels complete
    int current_level = 1;      // Current level (default starting level = 1)
    bool next_level = true;     // True to proceed to the next level in the next frame
    double timer = 0;           // Timer for multiplier powerup
    int score_multiplier = 1;   // Multiplier for score (default starting multiplier = 1)
    double paddle_x;            // Location of the paddle in the x axis
} game_data;

bitmap get_powerup_bitmap(powerups kind)
{
	switch(kind)
	{
	case MULTI_BALL:
		return bitmap_named("dropped_multi_ball");
	case SCORE_MULTIPLY:
		return bitmap_named("dropped_multiplier");
	default:
		return bitmap_named("dropped_multi_ball"); //default to multiball if kind is invalid
	}
}

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
            block.block_bitmap = "block_single_hit";
            break;
        case DOUBLE_HIT:
            block.hitpoint = 2;
            block.block_bitmap = "block_double_hit_1";
            break;
        case HIDDEN:
            block.hitpoint = 2;
            block.block_bitmap = "block_hidden_1";
            break;
    }
    switch (powerup)
    {
        case NO_POWERUP:
            block.powerup_bitmap = "block_hidden_1";
            break;
        case MULTI_BALL:
            block.powerup_bitmap = "block_multi_ball";
            break;
        case SCORE_MULTIPLY:
            block.powerup_bitmap = "block_multiplier";
            break;
        default:
            block.powerup_bitmap = "block_multi_ball"; //default if kind is invalid
            break;
    }
    block.powerup = powerup;
    return block;
}

ball_data create_ball(double x, double y, bool up, bool right)
{
	ball_data ball;
    ball.x = x;
    ball.y = y;
    ball.up = up;
    ball.right = right;
	return ball;
}

void draw_blocks()
{
    // Draw blocks to the screen
    for (int i = 0; i < game_data.blocks_in_level; i++)
    {
        if (game_data.blocks[i].powerup == SCORE_MULTIPLY)
        {
            switch (game_data.score_multiplier)
            {
                case 1:
                    game_data.blocks[i].powerup_bitmap = "block_multiplier";
                    break;
                case 2:
                    game_data.blocks[i].powerup_bitmap = "block_multiplier_3";
                    break;
                case 3:
                    game_data.blocks[i].powerup_bitmap = "block_multiplier_4";
                    break;
                default:
                    game_data.blocks[i].powerup_bitmap = "block_multiplier_5";
                    break;
            }
        }
		if (game_data.blocks[i].hitpoint > 0) //broken blocks aren't drawn
		{
            draw_bitmap(game_data.blocks[i].block_bitmap, game_data.blocks[i].x, game_data.blocks[i].y);
            draw_bitmap(game_data.blocks[i].powerup_bitmap, game_data.blocks[i].x, game_data.blocks[i].y);
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
        blocks[block_index].block_bitmap = "block_double_hit_2"; // Change color of bricks hit but not destroyed yet
    }
    else if (blocks[block_index].kind == HIDDEN)
    {
        blocks[block_index].block_bitmap = "block_hidden_2";
    }	
}

block_data * spawn_blocks_level1()
{
    game_data.remaining_blocks = BLOCKS_IN_LEVEL1; // Number of blocks remaining
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
block_data * spawn_blocks_level2()
{
    static block_data blocks[BLOCKS_IN_LEVEL2];
    game_data.remaining_blocks = BLOCKS_IN_LEVEL2;
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
block_data * spawn_blocks_level3()
{
    static block_data blocks[BLOCKS_IN_LEVEL3];
    game_data.remaining_blocks = BLOCKS_IN_LEVEL3;
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
block_data * spawn_blocks_level4()
{
    static block_data blocks[BLOCKS_IN_LEVEL4];
    game_data.remaining_blocks = BLOCKS_IN_LEVEL4;
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

void load_resources()
{
	load_resource_bundle("game_bundle", "bundle.txt");

    load_bitmap("ball", "ball.png");
    load_bitmap("paddle", "platform.png");

    load_bitmap("block_single_hit", "5.png");
    load_bitmap("block_double_hit_1", "8.png");
    load_bitmap("block_double_hit_2", "9.png");
    load_bitmap("block_hidden_1", "transparent.png");
    load_bitmap("block_hidden_2", "3.png");

    load_bitmap("block_multi_ball", "multiball_block.png");
    load_bitmap("block_multiplier", "multiplier_block.png");
    load_bitmap("block_multiplier_3", "multiplier_3_block.png");
    load_bitmap("block_multiplier_4", "multiplier_4_block.png");
    load_bitmap("block_multiplier_5", "multiplier_5_block.png");
	
	load_bitmap("dropped_multi_ball", "dropped_multiball.png");
	load_bitmap("dropped_multiplier", "dropped_multiplier.png"); 
}
void start_level()
{
    switch (game_data.current_level)
    {
        case 1:
            game_data.blocks = spawn_blocks_level1();
            game_data.blocks_in_level = BLOCKS_IN_LEVEL1;
            break;
        case 2: 
            game_data.blocks = spawn_blocks_level2();
            game_data.blocks_in_level = BLOCKS_IN_LEVEL2;
            break;
        case 3: 
            game_data.blocks = spawn_blocks_level3();
            game_data.blocks_in_level = BLOCKS_IN_LEVEL3;
            break;
        case 4:
            game_data.blocks = spawn_blocks_level4();
            game_data.blocks_in_level = BLOCKS_IN_LEVEL4;                    
            break;
        default:
            game_data.game_won = true;
            game_data.game_over = true;
            break;
    }
    // Reset score multiplier
    game_data.score_multiplier = 1;
    game_data.timer = 0;
        
    // Spawn ball at starting location
    game_data.current_balls.clear();
    game_data.current_balls.push_back(create_ball(screen_width()/2, 500, true, true));

    // Paddle starting location at the x axis
    game_data.paddle_x = (screen_width() - PADDLE_LENGTH) / 2;
    game_data.next_level = false;
}
void check_ball_collision(int i)
{
    // Bounce off walls
    if (game_data.current_balls[i].x - BALL_RADIUS <= 0) game_data.current_balls[i].right = true; // Bounce off left wall
    if (game_data.current_balls[i].x + BALL_RADIUS >= screen_width()) game_data.current_balls[i].right = false; // Bounce off right wall
    if (game_data.current_balls[i].y - BALL_RADIUS <= 0) game_data.current_balls[i].up = false; // Bounce off upper wall
    if (game_data.current_balls[i].y + BALL_RADIUS >= screen_height() and !game_data.game_over) // Hit bottom of the screen
    {
        //remove ball
        game_data.current_balls[i] = game_data.current_balls[game_data.current_balls.size()-1];
        game_data.current_balls.pop_back();
        
        if (game_data.current_balls.size() == 0) 
        {
            //lose the game if there are no more balls remaining
            game_data.game_won = false;
            game_data.game_over = true;
            
            game_data.current_powerups.clear(); // Remove existing powerup drops
        }
    }
    // Bounce off paddle
    if ((PADDLE_Y <= game_data.current_balls[i].y + BALL_RADIUS and game_data.current_balls[i].y + BALL_RADIUS <= PADDLE_Y + BALL_SPEED) and (game_data.current_balls[i].x + BALL_RADIUS >= game_data.paddle_x and game_data.current_balls[i].x - BALL_RADIUS <= game_data.paddle_x + PADDLE_LENGTH))
    {
        game_data.current_balls[i].up = true;
    }
    // Bounce off blocks
    for (int j = 0; j < game_data.blocks_in_level; j++)
    {
        bool collision = false;
        if (game_data.blocks[j].hitpoint > 0)
        {
            // Vertically
            if (game_data.current_balls[i].x >= game_data.blocks[j].x and game_data.current_balls[i].x <= game_data.blocks[j].x + BLOCK_WIDTH)
            {
                if (game_data.blocks[j].y + BLOCK_HEIGHT - BALL_SPEED <= game_data.current_balls[i].y - BALL_RADIUS and game_data.current_balls[i].y - BALL_RADIUS <= game_data.blocks[j].y + BLOCK_HEIGHT) // Bottom of block
                {
                    game_data.current_balls[i].up = false;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                    collision = true;
                }
                if (game_data.blocks[j].y <= game_data.current_balls[i].y + BALL_RADIUS and game_data.current_balls[i].y + BALL_RADIUS <= game_data.blocks[j].y + BALL_SPEED) // Top of block
                {
                    game_data.current_balls[i].up = true;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                    collision = true;
                }
            }
            // Horizontally
            if (game_data.current_balls[i].y + BALL_RADIUS >= game_data.blocks[j].y and game_data.current_balls[i].y - BALL_RADIUS < game_data.blocks[j].y + BLOCK_HEIGHT)
            {
                if (game_data.blocks[j].x + BLOCK_WIDTH <= game_data.current_balls[i].x - BALL_RADIUS - BALL_SPEED and game_data.current_balls[i].x - BALL_RADIUS <= game_data.blocks[j].x + BLOCK_WIDTH) // Right of block
                {
                    game_data.current_balls[i].right = true;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                    collision = true;
                }
                if (game_data.blocks[j].x <= game_data.current_balls[i].x + BALL_RADIUS and game_data.current_balls[i].x + BALL_RADIUS <= game_data.blocks[j].x + BALL_SPEED) // Left of block
                {
                    game_data.current_balls[i].right = false;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                    collision = true;
                }
            }
            if (collision and game_data.blocks[j].powerup != NO_POWERUP) // if the ball collided with any side of a powerup block
            {
                //create powerup drop
                powerup_drop_data new_powerup;
                new_powerup.x = game_data.blocks[j].x;
                new_powerup.y = game_data.blocks[j].y;
                new_powerup.kind = game_data.blocks[j].powerup;
                
                game_data.current_powerups.push_back(new_powerup);
            }
        }
    }
}
void update_ball_location(int i)
{
    if (game_data.current_balls[i].up) game_data.current_balls[i].y -= BALL_SPEED;
    else game_data.current_balls[i].y += BALL_SPEED;
    if (game_data.current_balls[i].right) game_data.current_balls[i].x += BALL_SPEED;
    else game_data.current_balls[i].x -= BALL_SPEED;
}

void draw_game()
{
    clear_screen(COLOR_BLACK);
    draw_blocks(); // Draw blocks
    for(int i = 0; i < game_data.current_balls.size(); i++) // Draw balls
    {
        //fill_circle(COLOR_WHITE, current_balls[i].x, current_balls[i].y, BALL_RADIUS);
        draw_bitmap("ball", game_data.current_balls[i].x, game_data.current_balls[i].y);
    }
    for(int i = 0; i < game_data.current_powerups.size(); i++) // Draw powerup drops
    {
        draw_bitmap(get_powerup_bitmap(game_data.current_powerups[i].kind),game_data.current_powerups[i].x,game_data.current_powerups[i].y);
    }
    draw_bitmap("paddle", game_data.paddle_x, PADDLE_Y); // Draw paddle
    draw_text("SCORE: " + to_string(game_data.score) + " MULTIPLIER: x" + to_string(game_data.score_multiplier) + " " + to_string(game_data.timer), COLOR_WHITE, font_named("default"), 20, 20, 20); // Draw score
}

int main()
{	
    open_window("DX Ball Game", 800, 600);
    window_toggle_border("DX Ball Game");

    // Load resources
    load_resources();

	// spawn only one block for testing winning a level
	/*
	block_data block; 
    block.x = 600;
    block.y = 300;
    blocks[index] = block;
	*/

    while(!key_down(ESCAPE_KEY))
    {
        if (game_data.timer > 0) game_data.timer -= (1.0 / 60.0); //count down 1/60 seconds every frame if the timer is in use
        else if (game_data.timer < 0) game_data.timer = 0;
        else game_data.score_multiplier = 1;
        
        // Start level
        if (game_data.next_level)
        {
            start_level();
        }

		process_events(); //check keyboard state
        // Player controls
		if (key_down(A_KEY) and game_data.paddle_x > 10) game_data.paddle_x -= PADDLE_SPEED; //moving left
		if (key_down(D_KEY) and game_data.paddle_x < screen_width() - PADDLE_LENGTH - 10) game_data.paddle_x += PADDLE_SPEED; //moving right
		
		for(int i = 0; i < game_data.current_balls.size(); i++)
		{
            check_ball_collision(i);
            update_ball_location(i);
		}
		
		for(int i = 0; i < game_data.current_powerups.size(); i++) //update powerup drops
		{
			// Update powerup drop locations
			game_data.current_powerups[i].y += BALL_SPEED/2; //powerup drops move at half the speed of balls
			
			//hit the bottom of the screen
			if(game_data.current_powerups[i].y + BALL_RADIUS >= screen_height())
			{
				//remove powerup
				game_data.current_powerups[i] = game_data.current_powerups[game_data.current_powerups.size()-1];
				game_data.current_powerups.pop_back();
			}
			//get caught by paddle
			if ((PADDLE_Y <= game_data.current_powerups[i].y + BALL_RADIUS and game_data.current_powerups[i].y + BALL_RADIUS <= PADDLE_Y + (BALL_SPEED/2)) and (game_data.current_powerups[i].x + BALL_RADIUS >= game_data.paddle_x and game_data.current_powerups[i].x - BALL_RADIUS <= game_data.paddle_x + PADDLE_LENGTH))
			{
				bool direction = (bool)rnd(0,2); //generate random 0 or 1 for horizontal direction

				//apply multi ball powerup
                if (game_data.current_powerups[i].kind == MULTI_BALL) game_data.current_balls.push_back(create_ball(game_data.current_powerups[i].x, PADDLE_Y + 10, true, direction)); //create ball above the paddle with a random horizontal direction
                //apply score multiplier powerup
                else if (game_data.current_powerups[i].kind == SCORE_MULTIPLY)
                {
                    game_data.timer = MULTIPLIER_DURATION; // set timer for MULTIPLIER_DURATION seconds
                    game_data.score_multiplier++;
                }
				//remove powerup drop
				game_data.current_powerups[i] = game_data.current_powerups[game_data.current_powerups.size()-1];
				game_data.current_powerups.pop_back();
			}
		}
		
		// Win level if all blocks are destroyed
		if (game_data.remaining_blocks == 0)
		{
			game_data.next_level = true;
            game_data.current_level++;
		}
        
        // Redraw everything
		draw_game();

		// Draw win/lose messages when level ends
		if (game_data.game_over)
		{
			end_level(game_data.game_won);

            if (key_typed(R_KEY)) //restart the game, can be moved to a function if we make a game data struct
            {
                game_data.score = 0;
                game_data.game_over = false;
                game_data.game_won = false;
                game_data.current_level = 1;
                game_data.next_level = false;
                game_data.timer = 0;
                game_data.score_multiplier = 1;

                // Spawn blocks
                game_data.blocks = spawn_blocks_level1();
                game_data.blocks_in_level = BLOCKS_IN_LEVEL1;

                // Spawn first ball at starting location
                game_data.current_balls.push_back(create_ball(screen_width() / 2, 500, true, true));

                // Paddle starting location at the x axis
                game_data.paddle_x = (screen_width() - PADDLE_LENGTH) / 2;

                // Draw the environment
                clear_screen(COLOR_BLACK);
                draw_blocks(); // Draw blocks
                draw_bitmap("paddle", game_data.paddle_x, PADDLE_Y); // Draw paddle
            }
		}

        // Shortcut button to change level for development purpose
        if (key_typed(F_KEY)) { game_data.next_level = true; game_data.current_level++; }

        refresh_screen(60);
    }
    return 0;
}
