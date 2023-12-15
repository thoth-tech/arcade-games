// Map Class
#include "splashkit.h"
#include "enemy.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include "block.h"
#include "collectable.h"
using namespace std;

#pragma once

/**
 * @brief Map class
 * 
 */
class LevelOjectsMap
{
    protected:
        vector<vector<int> > map_array;
        int tile_size = 64;
        int map_width;
        int map_height;
        string type;

    public:
        /**
         * @brief Default Constructor
         */
        LevelOjectsMap(){};

        /**
         * @brief Destructor
         */
        ~LevelOjectsMap()
        {
            //write_line("Deleting Map");
            map_array.clear();
        };

        LevelOjectsMap(string level, int tile_size)
        {
            this->tile_size = tile_size;
            check_map_dimensions(level);
            this->map_array = new_level(level);
        };
            
        void check_map_dimensions(string file)
        {
            ifstream map_level;
            std::string line;
            int y_count = 0;
            int x_count = 0;

            map_level.open(file);

            if(map_level.fail())
            {
                write_line("Error");
                cerr << "Error Opening File" << endl;
                exit(1);
            }

            while(std::getline(map_level, line))
            {
                std::istringstream iss(line);

                int i;
                x_count = 0;
                
                while(iss >> i)
                {
                    x_count += 1;
                }
                y_count += 1;
            }

            map_level.close();

            this->map_height = y_count;
            this->map_width = x_count;
        };

        int get_map_width()
        {
            return this->map_width;
        };

        int get_map_height()
        {
            return this->map_height;
        };

        vector<vector<int> > new_level(string file)
        {
            // Initialise a 2D matrix of int to store level design  
            vector<vector<int> > map;

            // load in the level layout from file
            ifstream map_level;
            map_level.open(file);

            // Check for errors
            if(map_level.fail())
            {
                write_line("Error");
                cerr << "Error Opening File" << endl;
                exit(1);
            }

            // initialise a vector of strings to store each line of text
            vector<int> map_line;
            int temp;

            for (int i = 0; i < this->map_height; i++)
            {
                for (int j = 0; j < this->map_width; j++)
                {
                    map_level >> temp;
                    map_line.push_back(temp);
                }
                // pushback line
                map.push_back(map_line);
                map_level.ignore();
                map_line.clear();
            }
            map_level.close();
            return map;
        };

        shared_ptr<Player> get_player_position(int player_number)
        {
            point_2d position;
            shared_ptr<Player> player;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 1201)
                    {
                        if(player_number == 1)
                        {
                            sprite player_sprite = create_sprite("blueGuy", "PlayerAnim");
                            player_input input = make_player1_input();
                            shared_ptr<Player> player(new Player(new IdleState, player_sprite, position, false, input));
                            player->set_player_id(1);
                            return player;
                        }
                        if(player_number == 3)
                        {
                            sprite player_sprite = create_sprite("purpleGuy", "PlayerAnim");
                            player_input input = make_player1_input();
                            shared_ptr<Player> player(new Player(new IdleState, player_sprite, position, false, input));
                            player->set_player_id(3);
                            return player;
                        }
                    }
                    if(this->map_array[i][j] == 1202)
                    {
                        if(player_number == 2)
                        {
                            sprite player_sprite = create_sprite("pinkGirl", "PlayerAnim");
                            player_input input = make_player2_input();
                            shared_ptr<Player> player(new Player(new IdleState, player_sprite, position, false, input));
                            player->set_player_id(2);
                            return player;
                        }
                    }
                }

            return player;
        };

        shared_ptr<DoorBlock> get_door(bitmap cell_sheet)
        {
            point_2d position;
            shared_ptr<DoorBlock> door;

            for (int i = 0; i < this->map_height; i++)
            {
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 1301)
                    {
                        shared_ptr<DoorBlock> level_door(new DoorBlock(cell_sheet, position));
                        door = level_door;
                    }
                }
            }

            return door;
        };
        

        vector<shared_ptr<Enemy>> get_enemies(vector<shared_ptr<Enemy>> level_enemies, vector<std::shared_ptr<Player>> level_players)
        {
            point_2d position;

             for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 1401)
                    {
                        sprite roach = create_sprite("Roach", "RoachAnim");
                        shared_ptr<Roach> cockroach(new Roach(roach, position, level_players));
                        cockroach->get_ai()->set_facing_left(false);
                        level_enemies.push_back(cockroach);
                    }
                    if(this->map_array[i][j] == 1402)
                    {
                        sprite roach = create_sprite("Roach", "RoachAnim");
                        shared_ptr<Roach> cockroach(new Roach(roach, position, level_players));
                        cockroach->get_ai()->set_facing_left(true);
                        level_enemies.push_back(cockroach);
                    }

                    if(this->map_array[i][j] == 1403)
                    {
                        sprite snake = create_sprite("Snake", "SnakeAnim");
                        shared_ptr<Snake> ssnake(new Snake(snake, position, level_players));
                        ssnake->get_ai()->set_facing_left(false);
                        level_enemies.push_back(ssnake);
                    }
                    if(this->map_array[i][j] == 1404)
                    {
                        sprite snake = create_sprite("Snake", "SnakeAnim");
                        shared_ptr<Snake> ssnake(new Snake(snake, position, level_players));
                        ssnake->get_ai()->set_facing_left(true);
                        level_enemies.push_back(ssnake);
                    }
                    if(this->map_array[i][j] == 1405)
                    {
                        sprite rat = create_sprite("Rat", "RatAnim");
                        shared_ptr<Rat> chubbyrat(new Rat(rat, position, level_players));
                        chubbyrat->get_ai()->set_facing_left(false);
                        level_enemies.push_back(chubbyrat);
                    }
                    if(this->map_array[i][j] == 1406)
                    {
                        sprite rat = create_sprite("Rat", "RatAnim");
                        shared_ptr<Rat> chubbyrat(new Rat(rat, position, level_players));
                        chubbyrat->get_ai()->set_facing_left(true);
                        level_enemies.push_back(chubbyrat);
                    }
                    if(this->map_array[i][j] == 1407)
                    {
                        sprite blob = create_sprite("Blob", "BlobAnim");
                        shared_ptr<Blob> bblob(new Blob(blob, position, level_players));
                        bblob->get_ai()->set_facing_left(true);
                        level_enemies.push_back(bblob);
                    }
                    if(this->map_array[i][j] == 1408)
                    {
                        sprite blob = create_sprite("Blob", "BlobAnim");
                        shared_ptr<Blob> bblob(new Blob(blob, position, level_players));
                        bblob->get_ai()->set_facing_left(false);
                        level_enemies.push_back(bblob);
                    }
                    if(this->map_array[i][j] == 1409)
                    {
                        sprite waterRat = create_sprite("WaterRat", "WaterRatAnim");
                        shared_ptr<WaterRat> WaterRatBoss(new WaterRat(waterRat, position, level_players));
                        WaterRatBoss->get_ai()->set_facing_left(true);
                        level_enemies.push_back(WaterRatBoss);
                    }
                    if(this->map_array[i][j] == 1410)
                    {
                        sprite waterRat = create_sprite("WaterRat", "WaterRatAnim");
                        shared_ptr<WaterRat> WaterRatBoss(new WaterRat(waterRat, position, level_players));
                        WaterRatBoss->get_ai()->set_facing_left(false);
                        level_enemies.push_back(WaterRatBoss);
                    }
                    if(this->map_array[i][j] == 1411)
                    {
                        sprite green_fly = create_sprite("FlyGreen", "FlyAnim");
                        shared_ptr<Fly> g_fly(new Fly(green_fly, position, level_players, "Green"));
                        g_fly->get_ai()->set_facing_left(true);
                        level_enemies.push_back(g_fly);
                    }
                    if(this->map_array[i][j] == 1412)
                    {
                        sprite green_fly = create_sprite("FlyGreen", "FlyAnim");
                        shared_ptr<Fly> g_fly(new Fly(green_fly, position, level_players, "Green"));
                        g_fly->get_ai()->set_facing_left(false);
                        level_enemies.push_back(g_fly);
                    }
                    if(this->map_array[i][j] == 1413)
                    {
                        sprite green_fly = create_sprite("FlyRed", "FlyAnim");
                        shared_ptr<Fly> g_fly(new Fly(green_fly, position, level_players, "Red"));
                        g_fly->get_ai()->set_facing_left(true);
                        level_enemies.push_back(g_fly);
                    }
                    if(this->map_array[i][j] == 1414)
                    {
                        sprite green_fly = create_sprite("FlyRed", "FlyAnim");
                        shared_ptr<Fly> g_fly(new Fly(green_fly, position, level_players, "Red"));
                        g_fly->get_ai()->set_facing_left(false);
                        level_enemies.push_back(g_fly);
                    }
                    if(this->map_array[i][j] == 1415)
                    {
                        sprite green_fly = create_sprite("FlyPurp", "FlyAnim");
                        shared_ptr<Fly> g_fly(new Fly(green_fly, position, level_players, "Purp"));
                        g_fly->get_ai()->set_facing_left(true);
                        level_enemies.push_back(g_fly);
                    }
                    if(this->map_array[i][j] == 1416)
                    {
                        sprite green_fly = create_sprite("FlyPurp", "FlyAnim");
                        shared_ptr<Fly> g_fly(new Fly(green_fly, position, level_players, "Purp"));
                        g_fly->get_ai()->set_facing_left(false);
                        level_enemies.push_back(g_fly);
                    }
                    if(this->map_array[i][j] == 1417)
                    {
                        sprite tent = create_sprite("Tentacle", "TentacleAnim");
                        shared_ptr<Tentacle> tentcle(new Tentacle(tent, position, level_players));
                        level_enemies.push_back(tentcle);
                    }
                }

            return level_enemies;
        };

        vector<shared_ptr<Block>> get_solid_blocks(vector<shared_ptr<Block>> solid_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Solid")
                        {
                            if(this->map_array[i][j] < bitmap_cell_count(cell_sheet) + 1)
                            {
                                shared_ptr<Block> block(new SolidBlock(cell_sheet, position, cell));
                                solid_blocks.push_back(block);
                            }
                        }
                        if(bitmap_name(cell_sheet) == "HalfBlocksTop")
                        {
                            if(this->map_array[i][j] < bitmap_cell_count(cell_sheet) + 1 + offset)
                            {
                                shared_ptr<Block> block(new HalfSolidBlockTop(cell_sheet, position, cell));
                                solid_blocks.push_back(block);
                            }
                        }
                        if(bitmap_name(cell_sheet) == "HalfBlocksBottom")
                        {
                            if(this->map_array[i][j] < bitmap_cell_count(cell_sheet) + 1 + offset)
                            {
                                shared_ptr<Block> block(new HalfSolidBlockBottom(cell_sheet, position, cell));
                                solid_blocks.push_back(block);
                            }
                        }
                    }
                }

            return solid_blocks;
        }

        vector<shared_ptr<EdgeBlock>> get_edges(vector<shared_ptr<EdgeBlock>> edge_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Edge")
                        {
                            if(this->map_array[i][j] < bitmap_cell_count(cell_sheet) + 1 + offset)
                            {
                                shared_ptr<EdgeBlock> block(new EdgeBlock(cell_sheet, position, cell));
                                edge_blocks.push_back(block);
                            }
                        }
                    }
                }

            return edge_blocks;
        }

        vector<shared_ptr<Block>> get_decoration(vector<shared_ptr<Block>> decoration_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Pipe")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new PipeBlock(cell_sheet, position, cell));
                                decoration_blocks.push_back(block);
                            }
                        }
                        if(bitmap_name(cell_sheet) == "Decorative")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new DecorativeBlock(cell_sheet, position, cell));
                                decoration_blocks.push_back(block);
                            }
                        }
                    }
                }

            return decoration_blocks;
        }

        vector<shared_ptr<WaterBlock>> get_water(vector<shared_ptr<WaterBlock>> water_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Water")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<WaterBlock> block(new WaterBlock(cell_sheet, position, cell));
                                water_blocks.push_back(block);
                            }
                        }
                    }
                }

            return water_blocks;
        }

        vector<shared_ptr<ToxicBlock>> get_toxic(vector<shared_ptr<ToxicBlock>> toxic_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Toxic")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<ToxicBlock> block(new ToxicBlock(cell_sheet, position, cell));
                                toxic_blocks.push_back(block);
                            }
                        }
                    }
                }

            return toxic_blocks;
        }

        vector<shared_ptr<Collectable>> get_collectables(vector<shared_ptr<Collectable>> collectables, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        //Less then 3?
                        if(bitmap_name(cell_sheet) == "Collect")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Collectable> block(new HeartCollectable(cell_sheet, position, cell));
                                collectables.push_back(block);
                            }
                        }
                    }
                }

            return collectables;
        }

        vector<shared_ptr<HoldablePipeBlock>> get_holdable_pipes(vector<shared_ptr<HoldablePipeBlock>> hold_pipes, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "HoldPipes")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<HoldablePipeBlock> block(new HoldablePipeBlock(cell_sheet, position, cell));
                                hold_pipes.push_back(block);
                            }
                        }
                    }
                }

            return hold_pipes;
        }

        vector<shared_ptr<TurnablePipeBlock>> get_turnable_pipes(vector<shared_ptr<TurnablePipeBlock>> hold_pipes, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "TurnPipes")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<TurnablePipeBlock> block(new TurnablePipeBlock(cell_sheet, position, cell));
                                hold_pipes.push_back(block);
                            }
                        }
                    }
                }

            return hold_pipes;
        }

        vector<shared_ptr<MultiTurnablePipeBlock>> get_multi_turnable_pipes(vector<shared_ptr<MultiTurnablePipeBlock>> hold_pipes, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "MultiPipes")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<MultiTurnablePipeBlock> block(new MultiTurnablePipeBlock(cell_sheet, position, cell));
                                hold_pipes.push_back(block);
                            }
                        }
                    }
                }

            return hold_pipes;
        }

        vector<shared_ptr<EmptyPipeBlock>> get_empty_pipe_blocks(vector<shared_ptr<EmptyPipeBlock>> empty_pipes, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "EmptyHold")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<EmptyPipeBlock> block(new EmptyPipeBlock(cell_sheet, position, cell));
                                empty_pipes.push_back(block);
                            }
                        }
                    }
                }

            return empty_pipes;
        }

        vector<shared_ptr<EmptyTurnBlock>> get_empty_turn_blocks(vector<shared_ptr<EmptyTurnBlock>> empty_pipes, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "EmptyTurn")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<EmptyTurnBlock> block(new EmptyTurnBlock(cell_sheet, position, cell));
                                empty_pipes.push_back(block);
                            }
                        }
                    }
                }

            return empty_pipes;
        }

        vector<shared_ptr<EmptyMultiTurnBlock>> get_empty_multi_turn_blocks(vector<shared_ptr<EmptyMultiTurnBlock>> empty_pipes, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "EmptyMulti")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<EmptyMultiTurnBlock> block(new EmptyMultiTurnBlock(cell_sheet, position, cell));
                                empty_pipes.push_back(block);
                            }
                        }
                    }
                }

            return empty_pipes;
        }
        

        vector<shared_ptr<Ladder>> get_ladders(vector<shared_ptr<Ladder>> ladder_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Ladder")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Ladder> block(new Ladder(cell_sheet, position, cell));
                                ladder_blocks.push_back(block);
                            }
                        }
                    }
                }

            return ladder_blocks;
        }
};

