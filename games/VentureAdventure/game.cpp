/*
 * This program has been written entirely 
 * By Anthony George - 220180567
 * Deakin University 
*/

#include "game.h"
#include "splashkit.h"
#include <iostream>
#include <fstream>

using namespace std;

game_data new_game()
{
    game_data new_game;

    // use new_level function to load the map to new_game.map_array
    new_game.map_array = new_level("level1.txt");

    // load the tiles to be used for the map design
    new_game.map = load_bitmap("tiles", "forest_tiles.png");
    bitmap_set_cell_details(new_game.map,32,32,16,16,256);

    new_game.player = new_player();

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
    ifstream map_level;

    map_level.open(file);

    // Check for errors
    if(map_level.fail())
    {
        cerr << "Error Opening File" << endl;
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
            if(game.map_array[i][j] > 1 && game.map_array[i][j] < 300)
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
void update_game(game_data &game)
{
    update_player(game.player);

    for (int i = 0; i < game.boxes.size(); i++)
        update_box(game.boxes[i], game.boxes[i].x_pos, game.boxes[i].y_pos);

    for (int i = 0; i < game.enemies.size(); i++)
        update_enemy(game.enemies[i]);

    handle_input(game);

    box_collision(game);

    box_wall_collision(game);

    box_box_collision(game);

    box_gem_collision(game);

    level_clear(game);

    moving(game);

    attack(game);

    hud(game);
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

    game.gems.push_back(gem);
}

void add_enemy(game_data &game, int x, int y)
{
    enemy_data enemy = new_enemy(x, y);

    enemy.id = game.enemies.size();

    game.enemies.push_back(enemy);
}

void remove_gem(game_data &game, int i)
{
    int gem_index = i;
    for(int j = 0; j < game.gems.size(); j++)
    {
        if(game.gems.size() == 1)
            break;

        else if(gem_index <= j)
        {
            swap(game.gems[gem_index], game.gems[j + 1]);
            gem_index++;
        }
        
    }
    game.gems.pop_back();
}

void swap(gem_data g1, gem_data g2)
{
    gem_data temp = g1;
    g1 = g2;
    g2 = temp;
}

void gem_collision(game_data &game)
{
    for (int i = 0; i < game.gems.size(); i++)
        if(sprite_collision(game.player.player_sprite, game.gems[i].gem_sprite))
        {
            play_sound_effect("diamond");
            game.player.gem += 1;
            remove_gem(game, i);
        }
}

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

void handle_input(game_data &game)
{   
    // initialise - get player position
    game.player.y_pos = sprite_y(game.player.player_sprite);
    game.player.x_pos = sprite_x(game.player.player_sprite);
    game.player.y_id  = game.player.y_pos/TILESIZE;
    game.player.x_id  = game.player.x_pos/TILESIZE;
    game.player.stopped = false;

    if(key_down(UP_KEY) && game.player.walking == false)
    {  
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

    if(key_down(DOWN_KEY) && game.player.walking == false)
    {
        sprite_start_animation(game.player.player_sprite, "stand_d");

        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x, game.solid[i].y - TILESIZE))
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_d");
                game.player.walking = true;        
            }

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

        if(game.player.move[LEFT] == false && game.player.move[RIGHT] == false
        && game.player.move[UP] == false && game.player.stopped == false)
            sprite_start_animation(game.player.player_sprite, "walk_down");

        if(game.player.walking == false) 
        {
            game.player.y_prev = game.player.y_pos;
            game.player.x_prev = game.player.x_pos;
            game.player.next = game.player.y_pos + TILESIZE;
            game.player.move[DOWN] = true;
            game.player.walking = true;
        }
    }

    if(key_down(LEFT_KEY) && game.player.walking == false)
    {
        sprite_start_animation(game.player.player_sprite, "stand_l");

        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x + TILESIZE, game.solid[i].y))
            {
               game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_l");
                game.player.walking = true;        
            }

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

        if(game.player.move[DOWN] == false && game.player.move[RIGHT] == false && game.player.move[UP] == false && game.player.stopped == false)
            sprite_start_animation(game.player.player_sprite, "walk_left");

        if(game.player.walking == false) 
        {
            game.player.y_prev = game.player.y_pos;
            game.player.x_prev = game.player.x_pos;
            game.player.next = game.player.x_pos - TILESIZE;
            game.player.move[LEFT] = true;
            game.player.walking = true;
        }   
    }

    if(key_down(RIGHT_KEY) && game.player.walking == false )
    {
        sprite_start_animation(game.player.player_sprite, "stand_r");

        for (int i = 0; i < game.solid.size(); i++)
            if(sprite_bitmap_collision(game.player.player_sprite, game.map, game.solid[i].x - TILESIZE, game.solid[i].y))
            {
                game.player.stopped = true;
                sprite_start_animation(game.player.player_sprite, "stand_r");
                game.player.walking = true;        
            }

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
            
        if(game.player.move[LEFT] == false && game.player.move[DOWN] == false && game.player.move[UP] == false && game.player.stopped == false)
            sprite_start_animation(game.player.player_sprite, "walk_right");
        

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

void box_collision(game_data &game)
{
    for(int i = 0; i < game.boxes.size(); i++)
    {
        if (sprite_collision(game.player.player_sprite, game.boxes[i].box_sprite))
        {
            if( game.player.move[UP] == true && game.boxes[i].up_stopped == false)
                sprite_set_y(game.boxes[i].box_sprite, sprite_y(game.player.player_sprite) - TILESIZE);

            if( game.player.move[DOWN] == true && game.boxes[i].down_stopped == false )
                sprite_set_y(game.boxes[i].box_sprite, sprite_y(game.player.player_sprite) + TILESIZE);

            if( game.player.move[LEFT] == true && game.boxes[i].left_stopped == false )
                sprite_set_x(game.boxes[i].box_sprite, sprite_x(game.player.player_sprite) - TILESIZE);

            if( game.player.move[RIGHT] == true && game.boxes[i].right_stopped == false )
                sprite_set_x(game.boxes[i].box_sprite, sprite_x(game.player.player_sprite) + TILESIZE);
        }
    }
}

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

void level_clear(game_data &game)
{   
    bitmap stairs = load_bitmap("stairs", "stairs.png");

    if(game.gems.size() == 0)
    {
        draw_bitmap(stairs, SCREEN_HEIGHT/2, SCREEN_WIDTH/2);
        
        if(sprite_bitmap_collision(game.player.player_sprite, stairs, SCREEN_HEIGHT/2, SCREEN_WIDTH/2))
        {
            sprite_set_x(game.player.player_sprite, game.player.x_prev);
            sprite_set_y(game.player.player_sprite, game.player.y_prev);
            sprite_start_animation(game.player.player_sprite, "stand_d");
            stop_music();
            play_sound_effect("win");
        }
    }
}

void start_screen()
{
    int ven = 500;
    int adv = 542;

    while( not key_down(SPACE_KEY) && not quit_requested() )
    {
        process_events();

        draw_bitmap("title", 0, 0);

        if(ven < SCREEN_HEIGHT/2-150) ven = SCREEN_HEIGHT/2-150;
        if(adv < SCREEN_HEIGHT/2-108 ) adv = SCREEN_HEIGHT/2-108;

        if(ven == SCREEN_HEIGHT/2-150)
        {
            draw_text("PRESS SPACE BAR TO START", COLOR_BLANCHED_ALMOND, "font.ttf", 20, SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT-50, option_to_screen());
            draw_text("By Anthony George", COLOR_BLANCHED_ALMOND, "font.ttf", 10, SCREEN_WIDTH/2+20, SCREEN_HEIGHT/2-50, option_to_screen());
        }

        draw_text("Venture", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH/2 - 35, ven = ven - 2, option_to_screen());
        draw_text("Adventure", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH/2 - 58, adv = adv - 2, option_to_screen());

        refresh_screen(60);
    }
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

void hud(game_data &game)
{
    draw_text("Level 1" , COLOR_BLACK, "font.ttf", 30, 17*TILESIZE, 0*TILESIZE);
    draw_text("Collect all the Gems" , COLOR_BLACK, "font.ttf", 20, 16*TILESIZE+5, 1*TILESIZE);
    draw_bitmap("hero", 17*TILESIZE, 4*TILESIZE, option_with_bitmap_cell(1));
    draw_text(" x 3" , COLOR_BLACK, "font.ttf", 20, 18*TILESIZE, 4*TILESIZE+10);
    draw_bitmap("gems", 17*TILESIZE, 5*TILESIZE, option_with_bitmap_cell(3));
    draw_text(" x "+ to_string(game.player.gem), COLOR_BLACK, "font.ttf", 20, 18*TILESIZE, 5*TILESIZE+10);
    
}

void start_debug(const game_data &game)
{
    write_line("===== Tile Info =====");
    write_line("Tile x loc: "+ to_string(static_cast<int>(game.solid[29].x)));
    write_line("Tile y loc: "+ to_string(static_cast<int>(game.solid[29].y)));
    write_line("tile x id: "   + to_string(static_cast<int>(game.solid[29].x/TILESIZE)));
    write_line("tile y id: "   + to_string(static_cast<int>(game.solid[29].y/TILESIZE)));
    write_line("=====================");
    write_line();
    write_line("==== Player Info =====");
    write_line("Player y_pos: "+ to_string(sprite_y(game.player.player_sprite)));
    write_line("Spider y pos: "+ to_string(sprite_y(game.enemies[0].enemy_sprite)));
    write_line("Player x id: "+ to_string(game.player.x_id));
    write_line("Player y id: "+ to_string(game.player.y_id));
    write_line("Player x pos: "+ to_string(game.player.x_pos));
    write_line("Player y pos: "+ to_string(game.player.y_pos));
    write_line("Player x prev: "+ to_string(game.player.x_prev));
    write_line("Player y prev: "+ to_string(game.player.y_prev));
    write_line("Player next: "+ to_string(game.player.next));
    write_line("Player up next: "+ to_string(game.player.up_next));
    write_line("Player down next: "+ to_string(game.player.down_next));
    write_line("Player left next: "+ to_string(game.player.left_next));
    write_line("Player right next: "+ to_string(game.player.right_next));
    write_line("Gems: "+ to_string(game.player.gem));
    write_line("=======================");
    write_line();
    write_line("====== Box Info 1 ======");
    write_line("Box x pos: "+ to_string(game.boxes[0].x_pos));
    write_line("Box y pos: "+ to_string(game.boxes[0].y_pos));
    write_line("Box x prev: "+ to_string(game.boxes[0].x_prev));
    write_line("Box y prev: "+ to_string(game.boxes[0].y_prev));
    write_line("Box next: "+ to_string(game.boxes[0].next));
    write_line("Box x id: "+ to_string(game.boxes[0].x_id));
    write_line("Box y id: "+ to_string(game.boxes[0].y_id));
    write_line("Box up next: "+ to_string(game.boxes[0].up_next));
    write_line("Box down next: "+ to_string(game.boxes[0].down_next));
    write_line("Box left next: "+ to_string(game.boxes[0].left_next));
    write_line("Box right next: "+ to_string(game.boxes[0].right_next));
    write_line("Box up stopped: "+ to_string(game.boxes[0].up_stopped));
    write_line("Box down stopped: "+ to_string(game.boxes[0].down_stopped));
    write_line("Box left stopped: "+ to_string(game.boxes[0].left_stopped));
    write_line("Box right stopped: "+ to_string(game.boxes[0].right_stopped));
    write_line("=======================");
    write_line();
    write_line("====== Box Info 2 ======");
    write_line("Box x pos: "+ to_string(game.boxes[1].x_pos));
    write_line("Box y pos: "+ to_string(game.boxes[1].y_pos));
    write_line("Box x id: "+ to_string(game.boxes[1].x_id));
    write_line("Box y id: "+ to_string(game.boxes[1].y_id));
    write_line("Box up next: "+ to_string(game.boxes[1].up_next));
    write_line("Box down next: "+ to_string(game.boxes[1].down_next));
    write_line("Box left next: "+ to_string(game.boxes[1].left_next));
    write_line("Box right next: "+ to_string(game.boxes[1].right_next));
    write_line("=======================");
    write_line();
    write_line("====== Gems ======");
    write_line("Gem x id: "+ to_string(game.gems[1].x_id));
    write_line("Gem y id: "+ to_string(game.gems[1].y_id));
    write_line("=======================");
}