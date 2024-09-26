#include "game.h"
#include "splashkit.h"
#include <iostream>
#include <fstream>
#include <math.h>

// TESTING and DEBUG TOOL
// make this 'true' to enable easier level completion, will cause only 1 gem to generate.
bool debugeasymode = false;

game_data new_game(string map)
{
    game_data new_game;

    // use new_level function to load the map to new_game.map_array
    new_game.map_array = new_level(map);


    // load the tiles to be used for the map design
    new_game.map = load_bitmap("tiles", "forest_tiles.png");
    bitmap_set_cell_details(new_game.map,32,32,16,16,256);

    // creates the player (player does not control number of lives or gem count)
    new_game.player = new_player();

    new_game.gemCount = 0;
    new_game.gameover = false;

    // pass in the map to be displayed 
    get_objects(new_game);

    for(int i = 0; i < TILESIZE*2; i++)
        new_game.index[i] = 0;
    
    return new_game;
};

// new_level function loads the text file contatining map design and layout
vector<vector<int> > new_level(string file)
{
    vector<vector<int> > map;

    // load in the level layout from file
    std::ifstream map_level;

    map_level.open(file);

    // Check for errors
    if(map_level.fail())
    {
        std::cerr << "Error Opening File" << std::endl;
        exit(1);
    }

    vector<int> map_line;
    int temp;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            map_level >> temp;
            map_line.push_back(temp);
        }

        map.push_back(map_line);
        map_level.ignore();
        map_line.clear();
    }
     
    return map;
}

// gets coordinates of all objects and if they are solid or not
void get_objects(game_data &game)
{
    int w = SCREEN_WIDTH /TILESIZE; //16
    int h = SCREEN_HEIGHT/TILESIZE; //16

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            // Tiles with numbers defined between 1 and 300 are 'solid'. 
            // Excludes 'enemy' currently defined as 200, and campfire(84), so that collision is possible. This if statement needs to be adjusted if more enemy tile numbers are added)
            if(game.map_array[i][j] > 1 && game.map_array[i][j] < 300 && game.map_array[i][j] != 200 && game.map_array[i][j] != 84)
            {
                int solid_x = j*TILESIZE;
                int solid_y = i*TILESIZE;
                game.map_loc = point_at(solid_x, solid_y);
                game.solid.push_back(game.map_loc);
            }

            if(game.map_array[i][j] == BOX)
                add_box(game, j*TILESIZE, i*TILESIZE);
            if(game.map_array[i][j] == GEM)
                add_gem(game, j*TILESIZE, i*TILESIZE);
            if(game.map_array[i][j] == ENEMY)
                add_enemy(game, j*TILESIZE, i*TILESIZE);
        }

}

// draws all objects
void draw_game(const game_data &game)
{
    int w = SCREEN_WIDTH /TILESIZE; //16
    int h = SCREEN_HEIGHT/TILESIZE; //16

    bitmap hud = load_bitmap("hud", "ground_tiles.png");
    bitmap_set_cell_details(hud,32,32,16,16,256);

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            // draw grass over whole map
            draw_bitmap(game.map, j*TILESIZE, i*TILESIZE, option_with_bitmap_cell(game.index[j]));
            // draw remaining map objects
            draw_bitmap(game.map, j*TILESIZE, i*TILESIZE, option_with_bitmap_cell(game.map_array[i][j]));
        }
    
    for (int i = 0; i < 17; i++)
    {
        int count = 0;
        for (int j = 16; j < 21; j++)
        {
            // draw hud
            draw_bitmap(hud, j*TILESIZE, i*TILESIZE, option_with_bitmap_cell(52));
            count++;
        }
    }

    draw_player(game.player);
    
    for (int i = 0; i < game.boxes.size(); i++)
        draw_box(game.boxes[i]);
    
    for (int i = 0; i < game.gems.size(); i++)
        draw_gem(game.gems[i]);

    for (int i = 0; i < game.enemies.size(); i++)
        draw_enemy(game.enemies[i]);
}

// updates all objects
bool update_game(game_data &game, int level_id, int lives)
{

    update_player(game.player);

    for (int i = 0; i < game.boxes.size(); i++)
        update_box(game.boxes[i], game.boxes[i].x_pos, game.boxes[i].y_pos);

    for (int i = 0; i < game.enemies.size(); i++)
        update_enemy(game.enemies[i]);

    // life decrements happen in program.cpp
    game.lives = lives;

    // large majority of player movement and player-environment collision checks are in handle_input
    handle_input(game);

    box_collision(game);

    box_wall_collision(game);

    box_box_collision(game);

    box_gem_collision(game);

    box_start_collision(game);

    enemy_collision(game);

    enemy_move(game);
    
    // updates win when level is clear
    bool level_completed = level_clear(game);

    moving(game);

    //Attack doesn't seem to be functional, CastielM has ended up creating different enemy collision functions
    //attack(game);

    draw_hud(game, level_id);

    return level_completed;
}

void add_box(game_data &game, int x, int y)
{
    box_data box = new_box(x, y);

    box.id = game.boxes.size();

    game.boxes.push_back(box);
}

void add_gem(game_data &game, int x, int y)
{
    gem_data gem = new_gem(x, y);

    gem.id = game.gems.size();
    gem.x_id = x/TILESIZE;
    gem.y_id = y/TILESIZE;

    // If easy mode is enabled, will only add 1 gem to the level
    if (debugeasymode == true)
    {
        if (game.gems.size() < 1)
        {
            game.gems.push_back(gem);
        }
    }
    //required to add all gems when easy mode not enabled
    else
    {
        game.gems.push_back(gem);
    }
    
}

void add_enemy(game_data &game, int x, int y)
{
    enemy_data enemy = new_enemy(x, y);

    enemy.id = game.enemies.size();

    // set starting direction to left
    enemy.dir[ELEFT] = true;

    game.enemies.push_back(enemy);

    // start walking left animation
    sprite_start_animation(enemy.enemy_sprite, "w_left");
    
    
}

void remove_gem(game_data &game, int i)
{
    int gem_index = i;
    for (int j = 0; j < game.gems.size(); j++)
    {
        if (game.gems.size() == 1)
            break;

        else if (gem_index <= j)
        {
            std::swap(game.gems[gem_index], game.gems[j + 1]);
            gem_index++;
        }
    }
    game.gems.pop_back();
}

// Unused - original author was accidentally using std::swap without realizing
// void swap(gem_data g1, gem_data g2)
// {
//     gem_data temp = g1;
//     g1 = g2;
//     g2 = temp;
// }

// checks for collision between player and gem
void gem_collision(game_data &game)
{
    for (int i = 0; i < game.gems.size(); i++)
    {
        if(sprite_collision(game.player.player_sprite, game.gems[i].gem_sprite))
        {
            play_sound_effect("diamond");
            game.gemCount += 1;
            remove_gem(game, i);
            // this is here to keep light_fire sound effect from looping, occurs when all gems collected
            if (game.gems.size() == 0)
            {
            play_sound_effect("light_fire");
            }
        }
        
    }
}

// collision check between player and enemy, removes lives and watches for game over conditions
void enemy_collision(game_data &game)
{

    for (int i = 0; i < game.enemies.size(); i++)
        if(sprite_collision(game.player.player_sprite, game.enemies[i].enemy_sprite))
        {
            // game over conditions
            if(game.lives == 0)
            {
            float vol = 0.3;
            play_sound_effect("game_over", vol);
            stop_music();
            draw_text("Game Over", COLOR_BLACK, "font.ttf", 70, SCREEN_WIDTH / 2 - 142, SCREEN_HEIGHT / 2 - 48, option_to_screen());
            draw_text("Game Over", COLOR_DARK_RED, "font.ttf", 70, SCREEN_WIDTH / 2 - 138, SCREEN_HEIGHT / 2 - 48, option_to_screen());
            game.gameover = true;
            }
            else{
            play_sound_effect("damage");
            game.lifelost = true;
            game.player = new_player();
            game.lives -= 1;
            }
        }
}

// checks all enemies for collision with solid tiles and adjusts direction accordingly. Currently only left/right movement.
// briefly attempted vertical movement but was having issues with animation getting stuck in solid tiles
void enemy_move(game_data &game)
{
    
    for (int i = 0; i < game.enemies.size(); i++)
    {
            update_enemy_position(game);

        for (int j = 0; j < game.solid.size(); j++)
        {
             // on collision with solid tile, switches direction
             if (sprite_bitmap_collision(game.enemies[i].enemy_sprite, game.map, game.solid[j].x, game.solid[j].y))
                {
                    if(game.enemies[i].dir[ELEFT] == true)
                    {
                        sprite_start_animation(game.enemies[i].enemy_sprite, "w_right");
                        game.enemies[i].dir[ELEFT] = false;
                        game.enemies[i].dir[ERIGHT] = true;

                    }
                    else if(game.enemies[i].dir[ERIGHT] == true)
                    {
                        sprite_start_animation(game.enemies[i].enemy_sprite, "w_left");
                        game.enemies[i].dir[ERIGHT] = false;
                        game.enemies[i].dir[ELEFT] = true;
                    }
                }
        }
   
    }

    //checks all enemies for collision with boxes and adjusts direction accordingly
    for (int i = 0; i < game.enemies.size(); i++)
    {
        for (int j = 0; j < game.boxes.size(); j++)
        {
             
             if (sprite_collision(game.enemies[i].enemy_sprite, game.boxes[j].box_sprite))
                {
                    if(game.enemies[i].dir[ELEFT] == true)
                    {
                        sprite_start_animation(game.enemies[i].enemy_sprite, "w_right");
                        //adjusts enemy x pos slightly after collision to prevent it from getting stuck in the box
                        sprite_set_x(game.enemies[i].enemy_sprite, game.enemies[i].x_pos + TILESIZE / 8);
                        game.enemies[i].dir[ELEFT] = false;
                        game.enemies[i].dir[ERIGHT] = true;

                    }
                    else if(game.enemies[i].dir[ERIGHT] == true)
                    {
                        sprite_start_animation(game.enemies[i].enemy_sprite, "w_left");
                        //adjusts enemy x pos slightly after collision to prevent it from getting stuck in the box
                        sprite_set_x(game.enemies[i].enemy_sprite, game.enemies[i].x_pos - TILESIZE / 8);
                        game.enemies[i].dir[ERIGHT] = false;
                        game.enemies[i].dir[ELEFT] = true;
                    }

                
                }
        }
   
    }
}


// Don't think this code is functional. Have commented it out in game update loop. Have developed other functions for enemies but leave this in in case it's useful in future
void attack(game_data &game)
{
    int box_id;

    for (int i = 0; i < game.enemies.size(); i++)
    {
        if(sprite_y(game.player.player_sprite) == sprite_y(game.enemies[i].enemy_sprite) && game.enemies[i].stopped == false)
        {
            sprite_start_animation(game.enemies[i].enemy_sprite, "w_left");

            for (int j = 0; j < game.boxes.size(); j++)
                if(game.boxes[j].y_pos == game.enemies[i].y_pos 
                && game.boxes[j].x_pos > game.player.x_pos)
                {
                    box_id = j;

                    if(game.boxes[box_id].y_pos == game.enemies[i].y_pos 
                    && game.boxes[box_id].x_pos > game.player.x_pos)
                        sprite_start_animation(game.enemies[i].enemy_sprite, "s_left");
                }

            if(sprite_collision(game.player.player_sprite, game.enemies[i].enemy_sprite) && game.enemies[i].stopped == false)
            {
                game.enemies[i].stopped = true;

                sprite_start_animation(game.enemies[i].enemy_sprite, "s_left");
                //sprite_set_x(game.enemies[i].enemy_sprite, game.enemies[i].x_pos + 32);
            
                sprite_start_animation(game.player.player_sprite, "dead");
                stop_music();

                //play_sound_effect("game_over");
                game.player.attacked = true;
            }
        }
    }
}

// large majority of player movement and player-environment collision happen here
void handle_input(game_data &game)
{   
    // initialise - get player position
    game.player.y_pos = sprite_y(game.player.player_sprite);
    game.player.x_pos = sprite_x(game.player.player_sprite);
    game.player.y_id  = game.player.y_pos/TILESIZE;
    game.player.x_id  = game.player.x_pos/TILESIZE;
    game.player.stopped = false;

    if(key_down(W_KEY) && game.player.walking == false)
    {  
        // checks for collision between box and enemy. specific based on player moving up (W key). stops box moving into enemy.
        // In future this could do possibly be changed to do something else, like destroy the enemy?
        update_enemy_position(game);
        update_box_position(game);
        for (int i = 0; i < game.boxes.size(); i++)
        {

            for (int j = 0; j < game.enemies.size(); j++)
            {
                // checks if the enemy 'next' tile (coming from any direction) is the same as the box 'next' tile, or if the enemy is already in the box 'next' tile
                // currently enemy can only be trapped between a minimum of 2 tiles. Was not able to the box closer without causing issues
                if((game.boxes[i].up_next == game.enemies[j].y_id && game.boxes[i].x_id == game.enemies[j].left_next)
                ||(game.boxes[i].up_next == game.enemies[j].y_id && game.boxes[i].x_id == game.enemies[j].right_next)
                ||(game.boxes[i].up_next == game.enemies[j].down_next && game.boxes[i].x_id == game.enemies[j].x_id)
                ||(game.boxes[i].up_next == game.enemies[j].y_id && game.boxes[i].x_id == game.enemies[j].x_id)
                ||(game.boxes[i].x_id == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].y_id))
                    {
                        game.boxes[i].up_stopped = true;
                    }
        }

        }
        sprite_start_animation(game.player.player_sprite, "stand_u");
        // stop at walls
        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x, game.solid[i].y + TILESIZE))
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_u");
                game.player.walking = true;        
            }

        // stop at stopped boxes
        for (int i = 0; i < game.boxes.size(); i++)
            if(game.player.up_next == game.boxes[i].y_id && game.player.x_id == game.boxes[i].x_id && game.boxes[i].up_stopped == true)
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_u");
                game.player.walking = true;

                for (int j = 0; j < game.solid.size(); j++)
                    if(game.boxes[i].up_next != game.solid[j].y)
                        game.boxes[i].up_stopped = false;
            }

        // walk
        if(game.player.move[LEFT] == false && game.player.move[RIGHT] == false && game.player.move[DOWN] == false && game.player.stopped == false && game.player.attacked == false)
            sprite_start_animation(game.player.player_sprite, "walk_up");

        // get info
        if(game.player.walking == false) 
        {
            game.player.y_prev = game.player.y_pos;
            game.player.x_prev = game.player.x_pos;
            game.player.next = game.player.y_pos - TILESIZE;
            game.player.move[UP] = true;
            game.player.walking = true;
        }
    }

    if(key_down(S_KEY) && game.player.walking == false)
    {
        // checks for collision between box and enemy. specific based on player moving up (W key). stops box moving into enemy
        update_enemy_position(game);
        update_box_position(game);
        for (int i = 0; i < game.boxes.size(); i++)
        {

            for (int j = 0; j < game.enemies.size(); j++)
            {
                 //checks if the enemy 'next' tile (coming from any direction) is the same as the box 'next' tile, or if the enemy is already in the box 'next' tile
                if((game.boxes[i].down_next == game.enemies[j].y_id && game.boxes[i].x_id == game.enemies[j].left_next)
                ||(game.boxes[i].down_next == game.enemies[j].y_id && game.boxes[i].x_id == game.enemies[j].right_next)
                ||(game.boxes[i].down_next == game.enemies[j].up_next && game.boxes[i].x_id == game.enemies[j].x_id)
                ||(game.boxes[i].down_next == game.enemies[j].y_id && game.boxes[i].x_id == game.enemies[j].x_id)
                ||(game.boxes[i].x_id == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].y_id))
                    {
                        game.boxes[i].down_stopped = true;
                    }
            }

        }
        
        sprite_start_animation(game.player.player_sprite, "stand_d");

        // stop at walls
        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x, game.solid[i].y - TILESIZE))
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_d");
                game.player.walking = true;        
            }

        // stop at stopped boxes
        for (int i = 0; i < game.boxes.size(); i++)
            if(game.player.down_next == game.boxes[i].y_id && game.player.x_id == game.boxes[i].x_id && game.boxes[i].down_stopped == true)
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_d");
                game.player.walking = true;

                for (int j = 0; j < game.solid.size(); j++)
                    if(game.boxes[i].down_next != game.solid[j].y)
                        game.boxes[i].down_stopped = false;
            }

        // walk
        if(game.player.move[LEFT] == false && game.player.move[RIGHT] == false
        && game.player.move[UP] == false && game.player.stopped == false)
            sprite_start_animation(game.player.player_sprite, "walk_down");

        // get info
        if(game.player.walking == false) 
        {
            game.player.y_prev = game.player.y_pos;
            game.player.x_prev = game.player.x_pos;
            game.player.next = game.player.y_pos + TILESIZE;
            game.player.move[DOWN] = true;
            game.player.walking = true;
        }
    }

    if(key_down(A_KEY) && game.player.walking == false)
    {
        // checks for collision between box and enemy. specific based on player moving left (A key). stops box moving into enemy
        update_enemy_position(game);
        update_box_position(game);
        for (int i = 0; i < game.boxes.size(); i++)
        {

            for (int j = 0; j < game.enemies.size(); j++)
            {

                //checks if the enemy 'next' tile (coming from any direction) is the same as the box 'next' tile, or if the enemy is already in the box 'next' tile
                if((game.boxes[i].left_next == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].down_next)
                ||(game.boxes[i].left_next == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].up_next)
                ||(game.boxes[i].left_next == game.enemies[j].right_next && game.boxes[i].y_id == game.enemies[j].y_id)
                ||(game.boxes[i].left_next == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].y_id)
                ||(game.boxes[i].x_id == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].y_id))
                    {
                        game.boxes[i].left_stopped = true;
                    }
            }

        }

        sprite_start_animation(game.player.player_sprite, "stand_l");

        // stop at walls
        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x + TILESIZE, game.solid[i].y))
            {
               game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_l");
                game.player.walking = true;        
            }

        // stop at stopped boxes
        for (int i = 0; i < game.boxes.size(); i++)
            if(game.player.left_next == game.boxes[i].x_id && game.player.y_id == game.boxes[i].y_id && game.boxes[i].left_stopped == true)
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_l");
                game.player.walking = true;

                for (int j = 0; j < game.solid.size(); j++)
                    if(game.boxes[i].left_next != game.solid[j].x)
                        game.boxes[i].left_stopped = false;
            }

        // walk
        if(game.player.move[DOWN] == false && game.player.move[RIGHT] == false && game.player.move[UP] == false && game.player.stopped == false)
            sprite_start_animation(game.player.player_sprite, "walk_left");

        // get info
        if(game.player.walking == false) 
        {
            game.player.y_prev = game.player.y_pos;
            game.player.x_prev = game.player.x_pos;
            game.player.next = game.player.x_pos - TILESIZE;
            game.player.move[LEFT] = true;
            game.player.walking = true;
        }   
    }

    if(key_down(D_KEY) && game.player.walking == false )
    {
        // checks for collision between box and enemy. specific based on player moving right (D key). stops box moving into enemy
        update_enemy_position(game);
        update_box_position(game);
        for (int i = 0; i < game.boxes.size(); i++)
        {

            for (int j = 0; j < game.enemies.size(); j++)
            {

                //checks if the enemy 'next' tile (coming from any direction) is the same as the box 'next' tile, or if the enemy is already in the box 'next' tile
                if((game.boxes[i].right_next == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].down_next)
                ||(game.boxes[i].right_next == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].up_next)
                ||(game.boxes[i].right_next == game.enemies[j].left_next && game.boxes[i].y_id == game.enemies[j].y_id)
                ||(game.boxes[i].right_next == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].y_id)
                ||(game.boxes[i].x_id == game.enemies[j].x_id && game.boxes[i].y_id == game.enemies[j].y_id))
                    {
                        game.boxes[i].right_stopped = true;
                    }
            }

        }

        sprite_start_animation(game.player.player_sprite, "stand_r");

        //stop at walls
        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x - TILESIZE, game.solid[i].y))
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_r");
                game.player.walking = true;        
            }

        // stop at stopped boxes
        for (int i = 0; i < game.boxes.size(); i++)
            if(game.player.right_next == game.boxes[i].x_id && game.player.y_id == game.boxes[i].y_id && game.boxes[i].right_stopped == true)
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_r");
                game.player.walking = true;

                for (int j = 0; j < game.solid.size(); j++)
                    if(game.boxes[i].right_next != game.solid[j].x)
                        game.boxes[i].right_stopped = false;
            }
        
        // walk
        if(game.player.move[LEFT] == false && game.player.move[DOWN] == false && game.player.move[UP] == false && game.player.stopped == false)
            sprite_start_animation(game.player.player_sprite, "walk_right");
        
        // get info
        if(game.player.walking == false) 
        {
            game.player.y_prev = game.player.y_pos;
            game.player.x_prev = game.player.x_pos;
            game.player.next = game.player.x_pos + TILESIZE;
            game.player.move[RIGHT] = true;
            game.player.walking = true;
        }  
    }
}

// keeps track of direction moved and correlating animations, along with players 'next' positions
void moving(game_data &game)
{
    if(game.player.walking == true)
    {       
        // collect gem
        gem_collision(game);


        if(game.player.y_prev > game.player.y_pos && game.player.y_pos <= game.player.next)
        {
            sprite_start_animation(game.player.player_sprite, "stand_u");
            sprite_set_y(game.player.player_sprite, game.player.next);
            
            game.player.move[UP] = false;
            game.player.walking = false;
            game.player.up_next = game.player.y_id - 1;
            game.player.down_next = game.player.y_id + 1;
        }
    
        if(game.player.y_prev < game.player.y_pos && game.player.y_pos >= game.player.next)
        {
            sprite_start_animation(game.player.player_sprite, "stand_d");
            sprite_set_y(game.player.player_sprite, game.player.next);
            
            game.player.move[DOWN] = false;
            game.player.walking = false;
            game.player.up_next = game.player.y_id - 1;
            game.player.down_next = game.player.y_id + 1;
        }

        if(game.player.x_prev > game.player.x_pos && game.player.x_pos <= game.player.next)
        {
            sprite_start_animation(game.player.player_sprite, "stand_l");
            sprite_set_x(game.player.player_sprite, game.player.next);
            
            game.player.move[LEFT] = false;
            game.player.walking = false;
            game.player.left_next = game.player.x_id - 1;
            game.player.right_next = game.player.x_id + 1;
        }

        if(game.player.x_prev < game.player.x_pos && game.player.x_pos >= game.player.next)
        {
            sprite_start_animation(game.player.player_sprite, "stand_r");
            sprite_set_x(game.player.player_sprite, game.player.next);
            
            game.player.move[RIGHT] = false;
            game.player.walking = false;
            game.player.right_next = game.player.x_id + 1;
            game.player.left_next = game.player.x_id - 1;
        }
    }
}

// allows boxes to be moved by player for each direction
void box_collision(game_data &game)
{
    game.lifelost = false;

    for(int i = 0; i < game.boxes.size(); i++)
    {
        if (sprite_collision(game.player.player_sprite, game.boxes[i].box_sprite))
        {   
            if( game.player.move[UP] == true && game.boxes[i].up_stopped == false)
             {  
                
                // checks for enemy collision again, if player is hit ('lifelost' variable), box will move either return to original tile or move to next tile depending on rounding
                // this fixes bug where box would get left sitting between tiles on death and cause a waterfall of issues
                enemy_collision(game);
                if (game.lifelost == true)
                {
                    game.boxes[i].y_id = round(sprite_y(game.boxes[i].box_sprite) / TILESIZE);
                    sprite_set_y(game.boxes[i].box_sprite, game.boxes[i].y_id * TILESIZE);
                }
                else
                {
                sprite_set_y(game.boxes[i].box_sprite, sprite_y(game.player.player_sprite) - TILESIZE);
                }
             }

            if( game.player.move[DOWN] == true && game.boxes[i].down_stopped == false )
            {  
                enemy_collision(game);
                if (game.lifelost == true)
                {
                    game.boxes[i].y_id = round(sprite_y(game.boxes[i].box_sprite) / TILESIZE);
                    sprite_set_y(game.boxes[i].box_sprite, game.boxes[i].y_id * TILESIZE);
                }
                else
                {
                sprite_set_y(game.boxes[i].box_sprite, sprite_y(game.player.player_sprite) + TILESIZE);;
                }
             }

            if( game.player.move[LEFT] == true && game.boxes[i].left_stopped == false )
            {  
                enemy_collision(game);
                if (game.lifelost == true)
                {
                    game.boxes[i].x_id = round(sprite_x(game.boxes[i].box_sprite) / TILESIZE);
                    sprite_set_x(game.boxes[i].box_sprite, game.boxes[i].x_id * TILESIZE);
                }
                else
                {
                sprite_set_x(game.boxes[i].box_sprite, sprite_x(game.player.player_sprite) - TILESIZE);
                }
             }


            if( game.player.move[RIGHT] == true && game.boxes[i].right_stopped == false )
            {  
                enemy_collision(game);
                if (game.lifelost == true)
                {
                    game.boxes[i].x_id = round(sprite_x(game.boxes[i].box_sprite) / TILESIZE);
                    sprite_set_x(game.boxes[i].box_sprite, game.boxes[i].x_id * TILESIZE);
                }
                else
                {
                sprite_set_x(game.boxes[i].box_sprite, sprite_x(game.player.player_sprite) + TILESIZE);
                }
             }
        }

    }
}

// checks for collision between box and solid tiles for each direction
void box_wall_collision(game_data &game)
{
    for (int i = 0; i < game.boxes.size(); i++)
    {   
        game.boxes[i].x_prev = sprite_x(game.boxes[i].box_sprite) - TILESIZE;
        game.boxes[i].y_prev = sprite_y(game.boxes[i].box_sprite) - TILESIZE;

        if(game.player.y_prev > game.player.y_pos && game.player.y_pos <= game.player.next)
        {
            game.boxes[i].y_pos = sprite_y(game.boxes[i].box_sprite);
            game.boxes[i].y_id = game.boxes[i].y_pos/TILESIZE;
            game.boxes[i].up_next = game.boxes[i].y_id - 1;
            game.boxes[i].down_next = game.boxes[i].y_id + 1;

            for (int j = 0; j < game.solid.size(); j++)
            {
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x, game.solid[j].y + TILESIZE))
                    game.boxes[i].up_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x + TILESIZE, game.solid[j].y))
                    game.boxes[i].left_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x - TILESIZE, game.solid[j].y))
                    game.boxes[i].right_stopped = true;
            }
        }

        if(game.player.y_prev < game.player.y_pos && game.player.y_pos >= game.player.next)
        {
            game.boxes[i].y_pos = sprite_y(game.boxes[i].box_sprite);
            game.boxes[i].y_id = game.boxes[i].y_pos/TILESIZE;
            game.boxes[i].up_next = game.boxes[i].y_id - 1;
            game.boxes[i].down_next = game.boxes[i].y_id + 1;

            for (int j = 0; j < game.solid.size(); j++)
            {
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x, game.solid[j].y - TILESIZE))
                    game.boxes[i].down_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x + TILESIZE, game.solid[j].y))
                    game.boxes[i].left_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x - TILESIZE, game.solid[j].y))
                    game.boxes[i].right_stopped = true;
            }
        }

        if(game.player.x_prev > game.player.x_pos && game.player.x_pos <= game.player.next)
        {
            game.boxes[i].x_pos = sprite_x(game.boxes[i].box_sprite);
            game.boxes[i].x_id = game.boxes[i].x_pos/TILESIZE;
            game.boxes[i].left_next  = game.boxes[i].x_id - 1;
            game.boxes[i].right_next = game.boxes[i].x_id + 1;

            for (int j = 0; j < game.solid.size(); j++)
            {
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x + TILESIZE, game.solid[j].y))
                    game.boxes[i].left_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x, game.solid[j].y + TILESIZE))
                    game.boxes[i].up_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x, game.solid[j].y - TILESIZE))
                    game.boxes[i].down_stopped = true;
            }
        }

        if(game.player.x_prev < game.player.x_pos && game.player.x_pos >= game.player.next)
        {
            game.boxes[i].x_pos = sprite_x(game.boxes[i].box_sprite);
            game.boxes[i].x_id = game.boxes[i].x_pos/TILESIZE;
            game.boxes[i].left_next  = game.boxes[i].x_id - 1;
            game.boxes[i].right_next = game.boxes[i].x_id + 1;

            for (int j = 0; j < game.solid.size(); j++)
            {
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x - TILESIZE, game.solid[j].y))
                    game.boxes[i].right_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x, game.solid[j].y + TILESIZE))
                    game.boxes[i].up_stopped = true;
                if(sprite_bitmap_collision(game.boxes[i].box_sprite, game.map, game.solid[j].x, game.solid[j].y - TILESIZE))
                    game.boxes[i].down_stopped = true;
            }
        }
    }
}

// prevents boxes being moved if there's another box in the way
void box_box_collision(game_data &game)
{
    for (int i = 0; i < game.boxes.size(); i++)            
        for (int j = 0; j < game.boxes.size(); j++)            
            if(i != j)
            {       
                if(game.boxes[i].up_next == game.boxes[j].y_id && game.boxes[i].x_id == game.boxes[j].x_id)
                    game.boxes[i].up_stopped = true;

                if(game.boxes[i].down_next == game.boxes[j].y_id && game.boxes[i].x_id == game.boxes[j].x_id)
                    game.boxes[i].down_stopped = true;

                if(game.boxes[i].left_next == game.boxes[j].x_id && game.boxes[i].y_id == game.boxes[j].y_id)
                    game.boxes[i].left_stopped = true;

                if(game.boxes[i].right_next == game.boxes[j].x_id && game.boxes[i].y_id == game.boxes[j].y_id)
                    game.boxes[i].right_stopped = true;
            }
}

// prevents box being pushed over gem
void box_gem_collision(game_data &game)
{
    for (int i = 0; i < game.boxes.size(); i++)            
        for (int j = 0; j < game.gems.size(); j++)     
        {             
            if(game.boxes[i].up_next == game.gems[j].y_id && game.boxes[i].x_id == game.gems[j].x_id)
                game.boxes[i].up_stopped = true;

            if(game.boxes[i].down_next == game.gems[j].y_id && game.boxes[i].x_id == game.gems[j].x_id)
                game.boxes[i].down_stopped = true;

            if(game.boxes[i].left_next == game.gems[j].x_id && game.boxes[i].y_id == game.gems[j].y_id)
                game.boxes[i].left_stopped = true;

            if(game.boxes[i].right_next == game.gems[j].x_id && game.boxes[i].y_id == game.gems[j].y_id)
                game.boxes[i].right_stopped = true;
        }
}

//prevents boxes from being able to be pushed over the player starting location (where they spawn if they lose a life). 8,8 position is hard coded in currently.
void box_start_collision(game_data &game)
{
    for (int i = 0; i < game.boxes.size(); i++)               
        {             
            if(game.boxes[i].up_next == 8 && game.boxes[i].x_id == 8)
                game.boxes[i].up_stopped = true;

            if(game.boxes[i].down_next == 8 && game.boxes[i].x_id == 8)
                game.boxes[i].down_stopped = true;

            if(game.boxes[i].left_next == 8 && game.boxes[i].y_id == 8)
                game.boxes[i].left_stopped = true;

            if(game.boxes[i].right_next == 8 && game.boxes[i].y_id == 8)
                game.boxes[i].right_stopped = true;
        }
}

// updates all enemy position date, including 'next' positions
void update_enemy_position (game_data &game)
{
    for (int i = 0; i < game.enemies.size(); i++)
    {
        game.enemies[i].x_pos = sprite_x(game.enemies[i].enemy_sprite);

        // enemy uses animation vectors to move, I think there were issues with rounding and so implementing this to fix it. Not ideal. Should animation be used for movement?
        // player also makes use of animation position changes
        game.enemies[i].x_id = ((TILESIZE/2) + game.enemies[i].x_pos) /TILESIZE;

        game.enemies[i].left_next = game.enemies[i].x_id - 1;
        game.enemies[i].right_next = game.enemies[i].x_id + 1;
        game.enemies[i].up_next = game.enemies[i].y_id - 1;
        game.enemies[i].down_next = game.enemies[i].y_id + 1;

    }
}

// updates all box position date, including 'next' positions
void update_box_position (game_data &game)
{
    for (int i = 0; i < game.boxes.size(); i++)
    {
        game.boxes[i].x_pos = sprite_x(game.boxes[i].box_sprite);
        game.boxes[i].x_id = ((TILESIZE/2) + game.boxes[i].x_pos) /TILESIZE;
        game.boxes[i].left_next = game.boxes[i].x_id - 1;
        game.boxes[i].right_next = game.boxes[i].x_id + 1;
        game.boxes[i].up_next = game.boxes[i].y_id - 1;
        game.boxes[i].down_next = game.boxes[i].y_id + 1;

    }
}

bool level_clear(game_data &game)
{   
    bitmap fire = load_bitmap("fire", "fire.png");

    if(game.gems.size() == 0)
    {   
        // draws lit fire bitmap in middle when all gems collected
        draw_bitmap(fire, SCREEN_HEIGHT/2, SCREEN_WIDTH/2);
        
        // level is complete when player returns to the fire
        if(sprite_bitmap_collision(game.player.player_sprite, fire, SCREEN_HEIGHT/2, SCREEN_WIDTH/2))
        {
            sprite_set_x(game.player.player_sprite, game.player.x_prev);
            sprite_set_y(game.player.player_sprite, game.player.y_prev);
            sprite_start_animation(game.player.player_sprite, "stand_d");
            stop_music();
            float vol = 0.2;
            play_sound_effect("level_win", vol);
            return true;
        }
    }
    return false;
}

bool check_gameover(game_data &game)
    {
        return game.gameover;
    }

int check_lives(game_data game)
    {
        return game.lives;
    }

void start_screen()
{
    int ven = 500;
    int adv = 542;

    while( not key_down(RETURN_KEY) && not quit_requested() && not key_down(ESCAPE_KEY))
    {
        process_events();

        draw_bitmap("title", 0, 0);

        if(ven < SCREEN_HEIGHT/2-150) 
            ven = SCREEN_HEIGHT/2-150;
        if(adv < SCREEN_HEIGHT/2-108 ) 
            adv = SCREEN_HEIGHT/2-108;

        if(ven == SCREEN_HEIGHT/2-150)
        {
            draw_text("PRESS ENTER KEY TO START", COLOR_BLANCHED_ALMOND, "font.ttf", 20, SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT-50, option_to_screen());
            draw_text("By Anthony George", COLOR_BLANCHED_ALMOND, "font.ttf", 10, SCREEN_WIDTH/2+20, SCREEN_HEIGHT/2-50, option_to_screen());
        }

        draw_text("Venture", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH/2 - 35, ven = ven - 2, option_to_screen());
        draw_text("Adventure", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH/2 - 58, adv = adv - 2, option_to_screen());

        refresh_screen(60);
    }
}

void win_screen()
{
    draw_text("You Win!", COLOR_BLACK, "font.ttf", 70, SCREEN_WIDTH / 2 - 112, SCREEN_HEIGHT / 2 - 48, option_to_screen());
    draw_text("You Win!", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH / 2 - 108, SCREEN_HEIGHT / 2 - 48, option_to_screen());
    refresh_screen(60);
}

void credits()
{
    int x = 500; 

    while( not quit_requested() )
    {
        process_events();
        clear_screen();

        draw_text("Venture", COLOR_BLACK, "font.ttf", 70, SCREEN_WIDTH/2 - 115, x = x - 0.01, option_to_screen());
        draw_text("Adventure", COLOR_BLACK, "font.ttf", 70, SCREEN_WIDTH/2 - 138, SCREEN_HEIGHT/2-48, option_to_screen());
        draw_text("PRESS SPACE BAR TO START", COLOR_BLACK, "font.ttf", 20, SCREEN_WIDTH/2 - 130, SCREEN_HEIGHT-140, option_to_screen());

        refresh_screen(60);
    }
}

void draw_hud(game_data &game, int level_id)
{
    string level_name = "Level " + std::to_string(level_id);

    draw_text(level_name , COLOR_BLACK, "font.ttf", 30, 17*TILESIZE, 0*TILESIZE);
    draw_text("Collect all the gems" , COLOR_BLACK, "font.ttf", 20, 16*TILESIZE+5, 1*TILESIZE);
    draw_text("and return to camp" , COLOR_BLACK, "font.ttf", 20, 16*TILESIZE+5, 2*TILESIZE);
    draw_bitmap("hero", 17*TILESIZE, 4*TILESIZE, option_with_bitmap_cell(1));
    draw_text(" x "+ std::to_string(game.lives) , COLOR_BLACK, "font.ttf", 20, 18*TILESIZE, 4*TILESIZE+10);
    draw_bitmap("gems", 17*TILESIZE, 5*TILESIZE, option_with_bitmap_cell(2));
    draw_text(" x "+ std::to_string(game.gemCount), COLOR_BLACK, "font.ttf", 20, 18*TILESIZE, 5*TILESIZE+10);
    draw_text("Move: ", COLOR_BLACK, "font.ttf", 20, 16*TILESIZE + 5, 9*TILESIZE);
    draw_text("WASD Keys /", COLOR_BLACK, "font.ttf", 20, 16*TILESIZE + 5, 10*TILESIZE);
    draw_text(" Joystick ", COLOR_BLACK, "font.ttf", 20, 19*TILESIZE - 5, 11*TILESIZE - 10);
    draw_text("Reset Map: ", COLOR_BLACK, "font.ttf", 20, 16*TILESIZE + 5, 12*TILESIZE);
    draw_text("R key /", COLOR_BLACK, "font.ttf", 20, 16*TILESIZE + 5, 13*TILESIZE);
    draw_text("Button 1 ", COLOR_BLACK, "font.ttf", 20, 18*TILESIZE, 14*TILESIZE - 10);
    
}