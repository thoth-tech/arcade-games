#include "splashkit.h"
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

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
const double NEW_BALL_DELAY = 5;	   // Delay between free new balls

// Input keys for player control
const key_code RIGHT = RIGHT_KEY;
const key_code LEFT = LEFT_KEY;
const key_code UP = UP_KEY;
const key_code DOWN = DOWN_KEY;
const key_code START = NUM_1_KEY;
const key_code ENTER = NUM_2_KEY;


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
    string characters[27] = {"-", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    int character1 = 0; // index of left initial
    int character2 = 0; // index of right initial
    int* current_character = &character1; // pointer to initial currently being edited
} initials_entry;

struct
{
    block_data *blocks;         // Array of blocks that exist in the current level
    vector<powerup_drop_data> current_powerups; // Vector of powerups currently dropping
	vector<ball_data> current_balls;            // Vector of balls currently active
    int remaining_blocks;       // Blocks remaining to destroy to complete level
    int blocks_in_level;        // Initial number of blocks
    int score = 0;              // Current score (default starting score = 0)
    bool game_start = false;    // True if player has started the game
    bool game_over = false;     // True if all levels complete or no more ball remaining
    bool game_won = false;      // True if all levels complete
    int current_level = 1;      // Current level (default starting level = 1)
    bool next_level = true;     // True to proceed to the next level in the next frame
    double timer = 0;           // Timer for multiplier powerup
	double extraBallTimer = 0;  // Timer for free new balls
    int score_multiplier = 1;   // Multiplier for score (default starting multiplier = 1)
    double paddle_x;            // Location of the paddle in the x axis
	json scores;				// JSON file storing data for the scoreboard
    json score_rows[10];        // Scoreboard rows from JSON file
    bool initials_entered = false; // Used to display scoreboard only after initials have been entered
} game_data;

bitmap get_powerup_bitmap(powerups kind)
{
	switch(kind)
	{
	case MULTI_BALL:
		return bitmap_named("dropped_multi_ball");
	case SCORE_MULTIPLY:
		if (game_data.score_multiplier < 5) return bitmap_named("dropped_multiplier_" + to_string(game_data.score_multiplier+1));
		else return bitmap_named("dropped_multiplier_5"); //prevent missing bitmap errors
		
	default:
		return bitmap_named("dropped_multi_ball"); //default to multiball if kind is invalid
	}
}

int high_score_position() // check if score is high enough to be on the scoreboard
{
    for (int i = 0; i < 10; i++)
    {
        if (json_read_number_as_int(game_data.score_rows[i], "score") < game_data.score)
        {
            return i; // return the row the score should be on
        }
    }
    return 10; // if score is lower than the score on row 9
}

void update_scores()
{
    int position = high_score_position();
    for (int i = 9; i > position; i--) // loops from the bottom of the scoreboard to the place of the new score
    {
        // replace each row up to the place of the new score with the row above, moving the rows down one place and removing the entry in last place
        json_set_number(game_data.score_rows[i], "score", json_read_number_as_int(game_data.score_rows[i - 1], "score"));
        json_set_string(game_data.score_rows[i], "initials", json_read_string(game_data.score_rows[i - 1], "initials"));
    }

    json_set_number(game_data.score_rows[position], "score", game_data.score); // set new score
    string initials = initials_entry.characters[initials_entry.character1] + initials_entry.characters[initials_entry.character2];
    json_set_string(game_data.score_rows[position], "initials", initials); // set new initials

    // update all rows and write new scoreboard to file
    for (int i = 0; i < 10; i++)
    {
        json_set_object(game_data.scores, "row" + to_string(i), game_data.score_rows[i]);
    }
    json_to_file(game_data.scores, "scores.json");
}

void end_game(bool successful)
{
    fill_rectangle(COLOR_LIGHT_GRAY, 200, 20, 400, 560); //draw the scoreboard background

    if (!game_data.initials_entered && high_score_position() != 10) // draw initial entry screen if current score is a new high score, until initials are submitted
    {
        draw_text("Enter initials", COLOR_BLACK, font_named("default"), 30, 280, 100);

        // draw initials
        draw_text(initials_entry.characters[initials_entry.character1], COLOR_BLACK, font_named("default"), 50, 360, 180);
        draw_text(initials_entry.characters[initials_entry.character2], COLOR_BLACK, font_named("default"), 50, 400, 180);

        // draw up and down arrows on initial being edited
        if (initials_entry.current_character == &initials_entry.character1)
        {
            fill_triangle(COLOR_BLACK, 374, 165, 364, 175, 384, 175);
            fill_triangle(COLOR_BLACK, 374, 240, 364, 230, 384, 230);
        }
        else
        {
            fill_triangle(COLOR_BLACK, 414, 165, 404, 175, 424, 175);
            fill_triangle(COLOR_BLACK, 414, 240, 404, 230, 424, 230);
        }

        draw_text("Press 2 to submit", COLOR_BLACK, font_named("default"), 30, 260, 280);

        //change initial
        if (key_typed(UP))
        {
            if (*initials_entry.current_character == 26) *initials_entry.current_character = 0; // loop back to '-' if going over maximum
            else *initials_entry.current_character = *initials_entry.current_character + 1;
        }
        else if (key_typed(DOWN))
        {
            if (*initials_entry.current_character == 0) *initials_entry.current_character = 26; // loop back to 'z' if going under minimum
            else *initials_entry.current_character = *initials_entry.current_character - 1;
        }
        //switch which initial is being edited
        else if (key_typed(LEFT) || key_typed(RIGHT))
        {
            if (initials_entry.current_character == &initials_entry.character1)
            {
                initials_entry.current_character = &initials_entry.character2;
            }
            else
            {
                initials_entry.current_character = &initials_entry.character1;
            }
        }
        if (key_typed(ENTER))
        {
            // submit initials
            game_data.initials_entered = true;
            update_scores();
        }
    }
    else // draw scoreboard
    {
        if (successful)
        {
            draw_text("You Win!", COLOR_RED, font_named("default"), 20, 340, 30);
        }
        else
        {
            draw_text("Game Over", COLOR_RED, font_named("default"), 20, 340, 30);
        }
        for (int i = 0; i <= 9; i++) { // draw scoreboard rows
            fill_rectangle(COLOR_GRAY, 220, 70 + 45 * i, 360, 40);
            draw_text(to_string(i + 1) + ".", COLOR_WHITE, font_named("default"), 18, 230, 80 + 45 * i); // draw row number
            draw_text(json_read_string(game_data.score_rows[i], "initials"), COLOR_WHITE, font_named("default"), 18, 260, 80 + 45 * i); // draw initials
            draw_text("score: " + to_string(json_read_number_as_int(game_data.score_rows[i], "score")), COLOR_WHITE, font_named("default"), 18, 370, 80 + 45 * i); // draw score
        }
        draw_text("Press 1 to play again", COLOR_BLACK, font_named("default"), 18, 275, 540);
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
            block.powerup_bitmap = "block_multiplier_2";
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
			if (game_data.score_multiplier < 5)
			{
				game_data.blocks[i].powerup_bitmap = ("block_multiplier_" + to_string(game_data.score_multiplier + 1)); 
			}
			else
			{
				game_data.blocks[i].powerup_bitmap = ("block_multiplier_5");
			}
			
        }
		if (game_data.blocks[i].hitpoint > 0) //broken blocks aren't drawn
		{
            draw_bitmap(game_data.blocks[i].block_bitmap, game_data.blocks[i].x, game_data.blocks[i].y);
            if (!(game_data.blocks[i].kind == HIDDEN && game_data.blocks[i].hitpoint > 1))
            {
                draw_bitmap(game_data.blocks[i].powerup_bitmap, game_data.blocks[i].x, game_data.blocks[i].y);
            }
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
        play_sound_effect("sfx_break_block");
        if (game_data.blocks[block_index].powerup != NO_POWERUP) // if the ball collided with any side of a powerup block
        {
            //create powerup drop
            powerup_drop_data new_powerup;
            new_powerup.x = game_data.blocks[block_index].x;
            new_powerup.y = game_data.blocks[block_index].y;
            new_powerup.kind = game_data.blocks[block_index].powerup;
            
            game_data.current_powerups.push_back(new_powerup);
        }
    }
    else if (blocks[block_index].kind == DOUBLE_HIT)
    {
        play_sound_effect("sfx_bounce_ball", (float) 0.25);
        blocks[block_index].block_bitmap = "block_double_hit_2"; // Change color of bricks hit but not destroyed yet
    }
    else if (blocks[block_index].kind == HIDDEN)
    {
        play_sound_effect("sfx_bounce_ball", (float) 0.25);
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
        block = create_block(160 + i * BLOCK_WIDTH, 240, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 6; i++)
    {
        block = create_block(220 + i * BLOCK_WIDTH, 260, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(220 + i * BLOCK_WIDTH, 220, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 4; i++)
    {
        block = create_block(280 + i * BLOCK_WIDTH, 280, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(280 + i * BLOCK_WIDTH, 200, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }
    for (int i = 0; i < 2; i++)
    {
        block = create_block(340 + i * BLOCK_WIDTH, 300, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
        block = create_block(340 + i * BLOCK_WIDTH, 180, SINGLE_HIT, NO_POWERUP);
        blocks[index] = block;
        index++;
    }

    for (int i = 0; i < 12; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL1;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = MULTI_BALL;
        blocks[index].powerup_bitmap = "block_multi_ball";
    }
    for (int i = 0; i < 5; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL1;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = SCORE_MULTIPLY;
        blocks[index].powerup_bitmap = "block_multiplier_2";
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

    for (int i = 0; i < 18; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL2;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = MULTI_BALL;
        blocks[index].powerup_bitmap = "block_multi_ball";
    }
    for (int i = 0; i < 8; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL2;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = SCORE_MULTIPLY;
        blocks[index].powerup_bitmap = "block_multiplier_2";
    }

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
            block = create_block(160 + j * BLOCK_WIDTH, 260 - i * BLOCK_HEIGHT, SINGLE_HIT, NO_POWERUP);
            blocks[index] = block;
            index++;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        block = create_block(40 + i * BLOCK_WIDTH, 320, HIDDEN, NO_POWERUP);
        blocks[index] = block;
        index++;
    }

    for (int i = 0; i < 20; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL3;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = MULTI_BALL;
        blocks[index].powerup_bitmap = "block_multi_ball";
    }
    for (int i = 0; i < 9; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL3;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = SCORE_MULTIPLY;
        blocks[index].powerup_bitmap = "block_multiplier_2";
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

    for (int i = 0; i < 50; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL4;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = MULTI_BALL;
        blocks[index].powerup_bitmap = "block_multi_ball";
    }
    for (int i = 0; i < 30; i++)
    {
        do
        {
            index = rand() % BLOCKS_IN_LEVEL4;
        } while (blocks[index].powerup != NO_POWERUP);
        blocks[index].powerup = SCORE_MULTIPLY;
        blocks[index].powerup_bitmap = "block_multiplier_2";
    }

    return blocks;
}

void load_resources()
{
	load_resource_bundle("game_bundle", "bundle.txt");

    load_bitmap("ball", "ball.png");
    load_bitmap("paddle", "platform.png");
    load_bitmap("title", "title.png");

    load_bitmap("block_single_hit", "5.png");
    load_bitmap("block_double_hit_1", "8.png");
    load_bitmap("block_double_hit_2", "9.png");
    load_bitmap("block_hidden_1", "transparent.png");
    load_bitmap("block_hidden_2", "3.png");

    load_bitmap("block_multi_ball", "multiball_block.png");
    load_bitmap("block_multiplier_2", "multiplier_block.png");
    load_bitmap("block_multiplier_3", "multiplier_3_block.png");
    load_bitmap("block_multiplier_4", "multiplier_4_block.png");
    load_bitmap("block_multiplier_5", "multiplier_5_block.png");
	
	load_bitmap("gauge_empty_2", "gauge_empty_2.png");
	load_bitmap("gauge_full_2", "gauge_full_2.png");
	load_bitmap("gauge_empty_3", "gauge_empty_3.png");
	load_bitmap("gauge_full_3", "gauge_full_3.png");
	load_bitmap("gauge_empty_4", "gauge_empty_4.png");
	load_bitmap("gauge_full_4", "gauge_full_4.png");
	load_bitmap("gauge_empty_5", "gauge_empty_5.png");
	load_bitmap("gauge_full_5", "gauge_full_5.png");
	
	load_bitmap("dropped_multi_ball", "dropped_multiball.png");
	load_bitmap("dropped_multiplier_2", "dropped_multiplier.png");
	load_bitmap("dropped_multiplier_3", "dropped_multiplier_3.png");
	load_bitmap("dropped_multiplier_4", "dropped_multiplier_4.png");
	load_bitmap("dropped_multiplier_5", "dropped_multiplier_5.png");
	
    load_sound_effect("sfx_break_block", "hurt_c_08-102842.wav");
    load_sound_effect("sfx_powerup", "video-game-powerup-38065.wav");
    load_sound_effect("sfx_bounce_ball", "stop-13692.wav");
    load_sound_effect("sfx_start_game", "message-incoming-132126.wav");
    load_sound_effect("sfx_win", "winsquare-6993.wav");
}
void show_title_screen()
{
    clear_screen(COLOR_BLACK);
    draw_bitmap("title", 0, 0);
    if (key_typed(START)) 
    {
        play_sound_effect("sfx_start_game");
        game_data.game_start = true;
    }
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
            play_sound_effect("sfx_win");
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

    // Remove powerups dropping from last level
    game_data.current_powerups.clear();

    // Paddle starting location at the x axis
    game_data.paddle_x = (screen_width() - PADDLE_LENGTH) / 2;
    game_data.next_level = false;
}
void reset_game()
{
    play_sound_effect("sfx_start_game");
	game_data.score = 0;
	game_data.game_over = false;
	game_data.game_won = false;
	game_data.current_level = 1;
	game_data.next_level = false;
	game_data.timer = 0;
	game_data.score_multiplier = 1;
    game_data.extraBallTimer = NEW_BALL_DELAY;

	//enable submission of another score
    game_data.initials_entered = false;
    initials_entry.character1 = 0;
    initials_entry.character2 = 0;

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
void check_ball_collision(int i)
{
    // Bounce off walls
    if (game_data.current_balls[i].x - BALL_RADIUS <= 0) 
    {
        play_sound_effect("sfx_bounce_ball", (float) 0.25);
        game_data.current_balls[i].right = true; // Bounce off left wall
    }
    if (game_data.current_balls[i].x + BALL_RADIUS >= screen_width()) 
    {
        play_sound_effect("sfx_bounce_ball", (float) 0.25);
        game_data.current_balls[i].right = false; // Bounce off right wall
    }
    if (game_data.current_balls[i].y - BALL_RADIUS <= 0) 
    {
        play_sound_effect("sfx_bounce_ball", (float) 0.25);
        game_data.current_balls[i].up = false; // Bounce off upper wall
    }
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
        play_sound_effect("sfx_bounce_ball", (float) 0.25);
        game_data.current_balls[i].up = true;
    }
    // Bounce off blocks
    for (int j = 0; j < game_data.blocks_in_level; j++)
    {
        if (game_data.blocks[j].hitpoint > 0)
        {
            // Vertically
            if (game_data.current_balls[i].x > game_data.blocks[j].x and game_data.current_balls[i].x < game_data.blocks[j].x + BLOCK_WIDTH)
            {
                if (game_data.blocks[j].y + BLOCK_HEIGHT - BALL_SPEED <= game_data.current_balls[i].y - BALL_RADIUS and game_data.current_balls[i].y - BALL_RADIUS <= game_data.blocks[j].y + BLOCK_HEIGHT) // Bottom of block
                {
                    game_data.current_balls[i].up = false;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                }
                if (game_data.blocks[j].y <= game_data.current_balls[i].y + BALL_RADIUS and game_data.current_balls[i].y + BALL_RADIUS <= game_data.blocks[j].y + BALL_SPEED) // Top of block
                {
                    game_data.current_balls[i].up = true;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                }
            }
            // Horizontally
            if (game_data.current_balls[i].y + BALL_RADIUS > game_data.blocks[j].y and game_data.current_balls[i].y - BALL_RADIUS < game_data.blocks[j].y + BLOCK_HEIGHT)
            {
                if (game_data.blocks[j].x + BLOCK_WIDTH <= game_data.current_balls[i].x - BALL_RADIUS - BALL_SPEED and game_data.current_balls[i].x - BALL_RADIUS <= game_data.blocks[j].x + BLOCK_WIDTH) // Right of block
                {
                    game_data.current_balls[i].right = true;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                }
                if (game_data.blocks[j].x <= game_data.current_balls[i].x + BALL_RADIUS and game_data.current_balls[i].x + BALL_RADIUS <= game_data.blocks[j].x + BALL_SPEED) // Left of block
                {
                    game_data.current_balls[i].right = false;
                    break_block(game_data.blocks, j, game_data.score, game_data.remaining_blocks, game_data.score_multiplier);
                }
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

void update_powerup_drops(int i)
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
        play_sound_effect("sfx_powerup",(float) 0.5);
		//apply multi ball powerup
		if (game_data.current_powerups[i].kind == MULTI_BALL) game_data.current_balls.push_back(create_ball(game_data.current_powerups[i].x, PADDLE_Y + 10, true, direction)); //create ball above the paddle with a random horizontal direction
		//apply score multiplier powerup
		else if (game_data.current_powerups[i].kind == SCORE_MULTIPLY)
		{
			game_data.timer = MULTIPLIER_DURATION; // set timer for MULTIPLIER_DURATION seconds
			if(game_data.score_multiplier < 5) game_data.score_multiplier++;
		}
		//remove powerup drop
		game_data.current_powerups[i] = game_data.current_powerups[game_data.current_powerups.size()-1];
		game_data.current_powerups.pop_back();
	}
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
    draw_text("SCORE:" + to_string(game_data.score), COLOR_WHITE, font_named("default"), 35, 20, 20); // Draw score
	if (game_data.score_multiplier > 1) //Draw multiplier guage
	{
		//Draw multiplier background		
		draw_bitmap("gauge_empty_" + to_string(game_data.score_multiplier), 170 + (20*floor(log10(game_data.score))), 24, option_scale_bmp(1,1));
		
		//Draw multiplier foreground
		bitmap filled_bitmap = bitmap_named("gauge_full_" + to_string(game_data.score_multiplier)); //Get bitmap with appropriate number
		rectangle bitmap_part = bitmap_bounding_rectangle(filled_bitmap); //Create bounding rectange for displaying part of the sprite's height
		bitmap_part.height = bitmap_height(filled_bitmap) * game_data.timer/10.0; //Shrink foreground as the timer goes down
		bitmap_part.y = bitmap_height(filled_bitmap) - bitmap_part.height; //Move foreground down to line up with background
		draw_bitmap(filled_bitmap, 171 + (20*floor(log10(game_data.score))), 25 + (bitmap_height(filled_bitmap) - bitmap_part.height), option_scale_bmp(1,1,option_part_bmp(bitmap_part))); //draw foreground
	}
}

int main()
{	
    open_window("DX Ball Game", 800, 600);
    window_toggle_border("DX Ball Game");

    // Load resources
    load_resources();

    // Load scores from JSON
    game_data.scores = json_from_file("scores.json");
    for (int i = 0; i < 10; i++)
    {
        game_data.score_rows[i] = json_read_object(game_data.scores, "row" + to_string(i));
    }

    while(!key_down(ESCAPE_KEY))
    {
		process_events(); //check keyboard state

        if(!game_data.game_start)
        {
            show_title_screen();
        }
        else
        {
            if (game_data.timer > 0) game_data.timer -= (1.0 / 60.0); //count down 1/60 seconds every frame if the timer is in use
            else if (game_data.timer < 0) game_data.timer = 0;
            else game_data.score_multiplier = 1;
			
			if (game_data.extraBallTimer > 0) game_data.extraBallTimer -= (1.0 / 60.0); //count down extra ball timer
			else if (!game_data.game_over)
			{
				int randomX = rnd(0 + BALL_RADIUS, 800 - BALL_RADIUS); //determine random x coordinate
				bool direction = (bool)rnd(0,2); //determine random direction
				game_data.current_balls.push_back(create_ball(randomX, 0 + BALL_RADIUS, false, direction)); //create new ball
				game_data.extraBallTimer = NEW_BALL_DELAY; //reset timer
			}

            
            // Start level
            if (game_data.next_level)
            {
                start_level();
            }

            // Player controls
            if (key_down(LEFT) and game_data.paddle_x > 10) game_data.paddle_x -= PADDLE_SPEED; //moving left
            if (key_down(RIGHT) and game_data.paddle_x < screen_width() - PADDLE_LENGTH - 10) game_data.paddle_x += PADDLE_SPEED; //moving right
            
            //update balls
            for(int i = 0; i < game_data.current_balls.size(); i++)
            {
                check_ball_collision(i);
                update_ball_location(i);
            }
            
            //update powerup drops
            for(int i = 0; i < game_data.current_powerups.size(); i++) //update powerup drops
            {
                update_powerup_drops(i);
            }
            
            // Redraw everything
            draw_game();

            // Win level if all blocks are destroyed
            if (game_data.remaining_blocks == 0  && game_data.current_level <= 4)
            {
                game_data.next_level = true;
                game_data.current_level++;
            }
            // Draw win/lose messages when level ends
            if (game_data.game_over)
            {
                end_game(game_data.game_won);
                
                if (key_typed(START)) reset_game();
            }

            // Shortcut button to change level for development purpose
            //if (key_typed(F_KEY)) { game_data.next_level = true; game_data.current_level++; }
            
            // Shortcut button to increase multiplier for development purpose
			/*
            if (key_typed(G_KEY))
            {
                game_data.timer = MULTIPLIER_DURATION;
                if(game_data.score_multiplier < 5) game_data.score_multiplier++;
            }
			*/
        }
        
        refresh_screen(60);
    }
    return 0;
}
