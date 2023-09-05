#include "splashkit.h"
#include "block.h"
#include "map.h"
#include "cellsheet.h"
#include "player.h"
#include "camera.h"
#include <memory>
#include <vector>

#pragma once

using namespace std;


vector<shared_ptr<Block>> make_level_solid_blocks(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Block>> solid_blocks;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        solid_blocks = map.get_solid_blocks(solid_blocks, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return solid_blocks;
}

vector<shared_ptr<Ladder>> make_level_ladders(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Ladder>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_ladders(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<WaterBlock>> make_level_water(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<WaterBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_water(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<ToxicBlock>> make_level_toxic(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<ToxicBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_toxic(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<HoldablePipeBlock>> make_holdable_pipes(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<HoldablePipeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_holdable_pipes(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<TurnablePipeBlock>> make_turnable_pipes(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<TurnablePipeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_turnable_pipes(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<MultiTurnablePipeBlock>> make_multi_turnable_pipes(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<MultiTurnablePipeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_multi_turnable_pipes(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<EmptyPipeBlock>> make_holdable_pipe_empty_spaces(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<EmptyPipeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_empty_pipe_blocks(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<EdgeBlock>> make_edges(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<EdgeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_edges(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<Collectable>> make_level_collectables(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Collectable>> collect;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        collect = map.get_collectables(collect, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return collect;
}

vector<shared_ptr<EmptyTurnBlock>> make_turnable_pipe_empty_spaces(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<EmptyTurnBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_empty_turn_blocks(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<EmptyMultiTurnBlock>> make_multi_turnable_pipe_empty_spaces(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<EmptyMultiTurnBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_empty_multi_turn_blocks(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<Block>> make_level_decoration(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Block>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_decoration(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<Enemy>> make_layer_enemies(vector<shared_ptr<Enemy>> level_enemy, string file, int tile_size, vector<std::shared_ptr<Player>> level_players)
{
    LevelOjectsMap map(file, tile_size);

    level_enemy = map.get_enemies(level_enemy, level_players);

    return level_enemy;
}

shared_ptr<Player> make_level_player(string file, int tile_size, int player_number)
{
    shared_ptr<Player> player;

    LevelOjectsMap map(file, tile_size);
    player = map.get_player_position(player_number);

    return player;
}

shared_ptr<DoorBlock> make_level_door(string file, int tile_size, bitmap cell_sheet)
{
    shared_ptr<DoorBlock> door;

    LevelOjectsMap map(file, tile_size);
    door = map.get_door(cell_sheet);

    return door;
}

shared_ptr<Camera> make_level_camera(shared_ptr<Player> player, string file, int tile_size)
{
    LevelOjectsMap map(file, tile_size);
    int map_width = map.get_map_width();
    int map_height = map.get_map_height();

    shared_ptr<Camera> camera(new Camera(player, tile_size, map_height, map_width));

    return camera;
}