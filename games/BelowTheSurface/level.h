#include "splashkit.h"
#include "block.h"
#include "camera.h"
#include "enemy.h"
#include "cellsheet.h"
#include "player.h"
#include "hud.h"
#include "map.h"
#include "testing.h"
#include "collision.h"
#include "background.h"
#include "levelparts.h"
#include <memory>
#include <vector>

#pragma once

class Level
{
    protected:
        vector<CellSheet> cell_sheets;
        vector<string> files;
        vector<shared_ptr<Player>> level_players;
        shared_ptr<DoorBlock> door;
        vector<shared_ptr<Enemy>> level_enemies;
        vector<vector<shared_ptr<Block>>> solid_blocks;
        vector<vector<shared_ptr<EdgeBlock>>> level_edges;
        vector<vector<shared_ptr<Ladder>>> ladders;
        vector<vector<shared_ptr<Block>>> decoration;
        vector<vector<shared_ptr<WaterBlock>>> water;
        vector<vector<shared_ptr<ToxicBlock>>> toxic;
        vector<vector<shared_ptr<HoldablePipeBlock>>> hold_pipes;
        vector<vector<shared_ptr<EmptyPipeBlock>>> empty_pipes;
        vector<vector<shared_ptr<TurnablePipeBlock>>> turn_pipes;
        vector<vector<shared_ptr<EmptyTurnBlock>>> empty_turn_pipes;
        vector<vector<shared_ptr<MultiTurnablePipeBlock>>> multi_turn_pipes;
        vector<vector<shared_ptr<EmptyMultiTurnBlock>>> empty_multi_turn_pipes;
        vector<vector<shared_ptr<Collectable>>> level_collectables;
        shared_ptr<Camera> camera;
        shared_ptr<Background> background;
        shared_ptr<HUD> level_hud;
        vector<string> pre_level_side_text;
        string password;
        int tile_size;
        int level_layers;
        int players;
        string level_name = "";
        music level_music;
        bitmap pre_level_image;

    public:
        bool is_player1_out_of_lives = false;
        bool player1_complete = false;

        bool is_player2_out_of_lives = false;
        bool player2_complete = true;

        Level(vector<CellSheet> cell_sheets, int tile_size, int players)
        {
            set_camera_x(0);
            set_camera_y(0);
            this->tile_size = tile_size;
            this->cell_sheets = cell_sheets;
            this->players = players;
            if (this->players == 2)
            {
                this->player2_complete = false;
                this->is_player2_out_of_lives = false;
            }
        };

        ~Level(){};

        void make_level()
        {
            this->door = make_level_door(files[0], this->tile_size, cell_sheets[5].cells);

            if (players == 2)
            {
                for (int i = 1; i < players + 1; i++)
                {
                    shared_ptr<Player> player = make_level_player(files[0], this->tile_size, i);
                    this->level_players.push_back(player);
                }
            }
            else
            {
                shared_ptr<Player> player = make_level_player(files[0], this->tile_size, 3);
                this->level_players.push_back(player);
            }

            for (int i = 0; i < level_layers; i++)
            {
                string file = files[i];

                vector<shared_ptr<Block>> solid_block;
                solid_block = make_level_solid_blocks(file, this->tile_size, this->cell_sheets);
                this->solid_blocks.push_back(solid_block);

                vector<shared_ptr<Ladder>> ladder_block;
                ladder_block = make_level_ladders(file, this->tile_size, this->cell_sheets);
                this->ladders.push_back(ladder_block);

                vector<shared_ptr<WaterBlock>> water_block;
                water_block = make_level_water(file, this->tile_size, this->cell_sheets);
                this->water.push_back(water_block);

                vector<shared_ptr<ToxicBlock>> toxic_block;
                toxic_block = make_level_toxic(file, this->tile_size, this->cell_sheets);
                this->toxic.push_back(toxic_block);

                vector<shared_ptr<HoldablePipeBlock>> holdpipe_block;
                holdpipe_block = make_holdable_pipes(file, this->tile_size, this->cell_sheets);
                this->hold_pipes.push_back(holdpipe_block);

                vector<shared_ptr<EmptyPipeBlock>> emp_block;
                emp_block = make_holdable_pipe_empty_spaces(file, this->tile_size, this->cell_sheets);
                this->empty_pipes.push_back(emp_block);

                vector<shared_ptr<TurnablePipeBlock>> turnpipe_block;
                turnpipe_block = make_turnable_pipes(file, this->tile_size, this->cell_sheets);
                this->turn_pipes.push_back(turnpipe_block);

                vector<shared_ptr<EmptyTurnBlock>> emp_turn_block;
                emp_turn_block = make_turnable_pipe_empty_spaces(file, this->tile_size, this->cell_sheets);
                this->empty_turn_pipes.push_back(emp_turn_block);

                vector<shared_ptr<MultiTurnablePipeBlock>> multiturnpipe_block;
                multiturnpipe_block = make_multi_turnable_pipes(file, this->tile_size, this->cell_sheets);
                this->multi_turn_pipes.push_back(multiturnpipe_block);

                vector<shared_ptr<EmptyMultiTurnBlock>> emp_multi_turn_block;
                emp_multi_turn_block = make_multi_turnable_pipe_empty_spaces(file, this->tile_size, this->cell_sheets);
                this->empty_multi_turn_pipes.push_back(emp_multi_turn_block);

                vector<shared_ptr<Block>> decoration_block;
                decoration_block = make_level_decoration(file, this->tile_size, this->cell_sheets);
                this->decoration.push_back(decoration_block);

                vector<shared_ptr<Collectable>> collect;
                collect = make_level_collectables(file, this->tile_size, this->cell_sheets);
                this->level_collectables.push_back(collect);

                vector<shared_ptr<EdgeBlock>> edges;
                edges = make_edges(file, this->tile_size, this->cell_sheets);
                this->level_edges.push_back(edges);

                this->level_enemies = make_layer_enemies(this->level_enemies, file, this->tile_size, this->level_players);
            }

            shared_ptr<HUD> hud(new HUD(level_players));
            this->level_hud = hud;

            this->camera = make_level_camera(level_players[0], files[0], tile_size);
        }

        void update()
        {
            clear_screen(COLOR_BLACK);
            background->draw();

            if (!music_playing())
            {
                play_music(this->level_music);
                set_music_volume(0.2f);
            }
                

            draw_layers(1, 0);

            door->draw_block();

            // Player functions
            for (int i = 0; i < level_players.size(); i++)
            {
                if (!level_players[i]->is_player_dead())
                {
                    level_players[i]->update();
                    level_players[i]->get_input();
                    level_players[i]->update_hitbox();
                }

                if (level_players[i]->has_player_won())
                {
                    if (i == 0)
                        player1_complete = true;
                    else
                        player2_complete = true;
                }
            }

            for (int i = 0; i < level_enemies.size(); i++)
            {
                if (rect_on_screen(level_enemies[i]->get_enemy_hitbox()))
                    level_enemies[i]->update();
                
                if(level_enemies[i]->get_is_boss())
                {
                    if(level_enemies[i]->get_dead())
                    {
                        player1_complete = true;
                        player2_complete = true;

                        for(int i = 0; i < level_players.size(); i++)
                        {
                            if(level_players[i]->get_state_type() != "Dance")
                                level_players[i]->change_state(new DanceState, "Dance");
                        }
                    }
                }
            }

            draw_layers(level_layers, 1);

            this->camera->update();
            check_collisions();

            for (int i = 0; i < level_players.size(); i++)
            {
                point_2d player_pos = sprite_position(level_players[i]->get_player_sprite());

                if(level_players[i]->get_player_id() == 2 && level_players[i]->get_state_type() == "Spawn")
                {
                    if(!point_on_screen(to_screen(player_pos)))
                    {
                        sprite_set_position(level_players[i]->get_player_sprite(), sprite_position(level_players[0]->get_player_sprite()));
                        player_pos = sprite_position(level_players[i]->get_player_sprite());
                    }
                }

                if(!point_on_screen(to_screen(player_pos)) && level_players[i]->get_state_type() != "Dying")
                {
                    if(level_players[i]->get_state_type() != "Spawn")
                        this->level_players[i]->change_state(new DyingState, "Dying");
                }

                //Player loses a life if they run out of health
                if (level_players[i]->player_health < 1 && level_players[i]->get_state_type() != "Dying")
                {
                    if(level_players[i]->get_state_type() != "Spawn")
                        this->level_players[i]->change_state(new DyingState, "Dying");
                }

                //If players sets out of lives
                if (level_players[i]->player_lives == 0 && level_players[i]->get_state_type() == "Spawn")
                {
                    this->level_players[i]->set_dead(true);

                    if (i == 0)
                        is_player1_out_of_lives = true;
                    else
                        is_player2_out_of_lives = true;
                }
            }

            level_hud->update();
        }

        void draw_layers(int num_layers, int start)
        {
            for(int j = start; j < num_layers; j++)
            {
                for(int i = 0; i < solid_blocks[j].size(); i++)
                    if(rect_on_screen(solid_blocks[j][i]->get_block_hitbox()))
                        solid_blocks[j][i]->draw_block();

                for(int i = 0; i < ladders[j].size(); i++)
                    if(rect_on_screen(ladders[j][i]->get_block_hitbox()))
                        ladders[j][i]->draw_block();

                for(int i = 0; i < decoration[j].size(); i++)
                    if(rect_on_screen(decoration[j][i]->get_block_hitbox()))
                        decoration[j][i]->draw_block();

                for(int i = 0; i < water[j].size(); i++)
                    //if(rect_on_screen(water[j][i]->get_block_hitbox()))
                        water[j][i]->draw_block();
                
                for(int i = 0; i < toxic[j].size(); i++)
                    if(rect_on_screen(toxic[j][i]->get_block_hitbox()))
                        toxic[j][i]->draw_block();

                for(int i = 0; i < hold_pipes[j].size(); i++)
                    if(rect_on_screen(hold_pipes[j][i]->get_block_hitbox()))
                        hold_pipes[j][i]->draw_block();
                
                for(int i = 0; i < empty_pipes[j].size(); i++)
                    if(rect_on_screen(empty_pipes[j][i]->get_block_hitbox()))
                        empty_pipes[j][i]->draw_block();

                for(int i = 0; i < turn_pipes[j].size(); i++)
                    if(rect_on_screen(turn_pipes[j][i]->get_block_hitbox()))
                        turn_pipes[j][i]->draw_block();
                
                for(int i = 0; i < empty_turn_pipes[j].size(); i++)
                    //if(rect_on_screen(empty_turn_pipes[j][i]->get_block_hitbox()))
                        empty_turn_pipes[j][i]->draw_block();

                for(int i = 0; i < multi_turn_pipes[j].size(); i++)
                    if(rect_on_screen(multi_turn_pipes[j][i]->get_block_hitbox()))
                        multi_turn_pipes[j][i]->draw_block();

                for(int i = 0; i < empty_multi_turn_pipes[j].size(); i++)
                    //if(rect_on_screen(empty_multi_turn_pipes[j][i]->get_block_hitbox()))
                        empty_multi_turn_pipes[j][i]->draw_block();
                
                for(int i = 0; i < level_collectables[j].size(); i++)
                    if(rect_on_screen(level_collectables[j][i]->get_hitbox()))
                        level_collectables[j][i]->draw();

                for(int i = 0; i < level_edges[j].size(); i++)
                    if(rect_on_screen(level_edges[j][i]->get_block_hitbox()))
                        level_edges[j][i]->draw_block();
            }
        }

        void check_collisions()
        {
            check_ladder_collisions(ladders, level_players);
            check_solid_block_collisions(solid_blocks, level_players);

            // check for player to pick up a holdable pipe
            check_holdable_pipe_block_collisions(hold_pipes, level_players);
            check_turnable_pipe_block_collisions(turn_pipes, level_players);

            // check for player to place it's pipe on th empty pipe
            check_empty_pipe_block_collisions(empty_pipes, level_players);
            check_door_block_collisions(door, level_players);
            check_enemy_solid_block_collisions(solid_blocks, level_enemies);
            check_enemy_edge_block_collisions(level_edges, level_enemies);
            check_enemy_player_collisions(level_enemies, level_players);
            check_water_block_collisions(water, level_players);
            check_toxic_block_collisions(toxic, level_players);
            check_water_water_block_collisions(water, water);
            check_water_empty_block_collisions(empty_pipes, water);
            check_water_empty_turn_block_collisions(empty_turn_pipes, water);
            check_turn_empty_pipes(turn_pipes, empty_turn_pipes);
            check_multi_turnable_pipe_block_collisions(multi_turn_pipes, level_players);
            check_water_empty_multi_turn_block_collisions(empty_multi_turn_pipes, water);
            check_turn_multi_empty_pipes(multi_turn_pipes, empty_multi_turn_pipes);
            check_collectable_collisions(level_collectables, level_players);
        }

        string get_level_name()
        {
            return this->level_name;
        };

        string get_level_password()
        {
            return this->password;
        };

        vector<string> get_pre_level_text()
        {
            return pre_level_side_text;
        };

        bitmap get_pre_level_image()
        {
            return pre_level_image;
        };
};

class Level0 : public Level
{
    public:
        Level0(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/level0_1.txt");
            this->files.push_back("levels/level0_2.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Fix those leaks!";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "START";
            this->pre_level_image = bitmap_named("level1");
        };
};

class MultiPipe : public Level
{
    public:
        MultiPipe(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 4;
            this->files.push_back("levels/multi_1.txt");
            this->files.push_back("levels/multi_2.txt");
            this->files.push_back("levels/multi_3.txt");
            this->files.push_back("levels/multi_4.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Multi-Pipe Madness";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "MULTI";
            this->pre_level_side_text.push_back("Into The Sewers..");
            this->pre_level_side_text.push_back("Hoping To Find..");
            this->pre_level_side_text.push_back("The Answers..");
            this->pre_level_image = bitmap_named("level2");
        };
};



class TooManyRoach : public Level
{
    public:
        TooManyRoach(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/roach_1.txt");
            this->files.push_back("levels/roach_2.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Too Many Roaches";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "ROACH";
            this->pre_level_side_text.push_back("Never Too Many");
            this->pre_level_image = bitmap_named("roachpl");
        };
};

class Level3 : public Level
{
    public:
        Level3(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/level3_1.txt");
            this->files.push_back("levels/level3_2.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Turn and Climb Time";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "CLIMB";
            this->pre_level_side_text.push_back("Turn all those pipes!");
            this->pre_level_image = bitmap_named("level4");
        };
};

class FourCorners : public Level
{
    public:
        FourCorners(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 4;
            this->files.push_back("levels/4c_1.txt");
            this->files.push_back("levels/4c_2.txt");
            this->files.push_back("levels/4c_3.txt");
            this->files.push_back("levels/4c_4.txt");
            make_level();
            this->level_music = music_named("ThothTemple.mp3");
            this->level_name = "The 4 Trials of Thoth";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "TEMPL";
            this->pre_level_side_text.push_back("Turn The Pipe");
            this->pre_level_side_text.push_back("In Each Corner");
            this->pre_level_side_text.push_back("To Defeat The Trials");
            this->pre_level_image = bitmap_named("level6");
        };
};

class Rats : public Level
{
    public:
        Rats(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/rats0.txt");
            this->files.push_back("levels/rats1.txt");
            make_level();
            this->level_music = music_named("ThothTemple.mp3");
            this->level_name = "Rats'Trap";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "MOUSE";
            this->pre_level_side_text.push_back("Thoth Tech's acolytes...");
            this->pre_level_side_text.push_back("This is a trap!");
            this->pre_level_image = bitmap_named("level7");
        };
};

class Surf : public Level
{
    public:
        Surf(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/surf_1.txt");
            this->files.push_back("levels/surf_2.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Slime Surfin'";
            shared_ptr<Background> backg(new DarkBackground);
            this->background = backg;
            this->password = "SURFN";
            this->pre_level_side_text.push_back("Surfin' through the");
            this->pre_level_side_text.push_back("Slime");
            this->pre_level_image = bitmap_named("level8");
        };
};

class Level6  : public Level
{
    public:
        Level6(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 3;
            this->files.push_back("levels/level6_1.txt");
            this->files.push_back("levels/level6_2.txt");
            this->files.push_back("levels/level6_3.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Jumpingworld!";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "JUMPW";
            this->pre_level_side_text.push_back("Jump for the pipe...");
            this->pre_level_side_text.push_back("...But be careful");
            this->pre_level_image = bitmap_named("level5");
        };
};

class Easy  : public Level
{
    public:
        Easy(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 3;
            this->files.push_back("levels/easy1.txt");
            this->files.push_back("levels/easy2.txt");
            this->files.push_back("levels/easy3.txt");
            make_level();
            this->level_music = music_named("MenuMusic.mp3");//menu
            this->level_name = "Easy...enough";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "EZPZ";
            this->pre_level_side_text.push_back("Climb to the top...");
            this->pre_level_side_text.push_back("Watch out for snakes!");
            this->pre_level_image = bitmap_named("level3");
        };
};

class CombatLevel : public Level
{
    public:
        CombatLevel(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/combat_level0.txt");
            this->files.push_back("levels/combat_level1.txt");
            make_level();
            this->level_music = music_named("BossMusic.mp3");
            this->level_name = "Combat Level";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "FIGHT";
            this->pre_level_side_text.push_back("Fight your way out!");
            this->pre_level_image = bitmap_named("level9");
        };
};

class BossLevel : public Level
{
    public:
        BossLevel(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/boss1.txt");
            this->files.push_back("levels/boss2.txt");
            make_level();
            this->level_music = music_named("BossMusic.mp3");
            this->level_name = "Boss Fight";
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            this->password = "BOSS";
            this->pre_level_side_text.push_back("Water Rat is angry!");
            this->pre_level_side_text.push_back("...watch out!");
            this->pre_level_image = bitmap_named("level9");
        };
};

class Mario  : public Level
{
    public:
        Mario(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/mario1.txt");
            this->files.push_back("levels/mario2.txt");
            make_level();
            this->level_music = music_named("Mario");
            this->level_name = "M... Mario?..";
            shared_ptr<Background> backg(new SkyBackground);
            this->background = backg;
            this->password = "MARIO";
            this->pre_level_side_text.push_back("How Did");
            this->pre_level_side_text.push_back("I Get Here?..");
            this->pre_level_image = bitmap_named("mariopl");
            change_physics();
            change_sprites();
        };

        void change_physics()
        {
            for(int i = 0; i < level_players.size(); i++)
            {
                level_players[i]->max_jump_height = 200;
                level_players[i]->jump_momentum_rate = 20;
            }
        };

        void change_sprites()
        {
            for(int i = 0; i < level_players.size(); i++)
            {
                if(i == 0)
                {
                    point_2d pos = level_players[i]->get_player_position();
                    sprite player_sprite = create_sprite("redGuy", "PlayerAnim");
                    sprite_set_position(player_sprite, pos);
                    level_players[i]->set_player_sprite(player_sprite);
                }
                else
                {
                    point_2d pos = level_players[i]->get_player_position();
                    sprite player_sprite = create_sprite("greenGuy", "PlayerAnim");
                    sprite_set_position(player_sprite, pos);
                    level_players[i]->set_player_sprite(player_sprite);
                }
            }
        };
};

class BlankLevel : public Level
{
    public:
        BlankLevel(vector<CellSheet> cell_sheets, int tile_size, int players, int layers, vector<string> file_names) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = layers - 1;
            for (int i = 0; i < layers; i++)
            {  
                if(i < layers - 1)
                    this->files.push_back(file_names[i]);
                else
                    this->level_name = file_names[i];
            }
            this->level_music = music_named("LevelOne");
            shared_ptr<Background> backg(new GreyBackground);
            this->background = backg;
            make_level();
            this->password = "";
            this->pre_level_image = bitmap_named("SlimeSurf");
        };
};