#include "splashkit.h"
#include "cellsheet.h"
#include "screen.h"
#include "enemy.h"
#include "testing.h"
#include <memory>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 896
#define TILE_SIZE 64

vector<CellSheet> make_cell_sheets(vector<string> cell_sheet_names)
{
    vector<CellSheet> cell_sheets;

    int offset = 0;
    for (int i = 0; i < cell_sheet_names.size(); i++)
    {
        bitmap new_bitmap = bitmap_named(cell_sheet_names[i]);
        CellSheet new_type(new_bitmap, cell_sheet_names[i], offset);
        offset += 100;
        cell_sheets.push_back(new_type);
    }

    return cell_sheets;
}

int main(int argc, char *argv[])
{
    // Load Resources
    load_resource_bundle("player", "playerbundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    load_resource_bundle("roach", "roachbundle.txt");
    load_resource_bundle("snake", "snakebundle.txt");
    load_resource_bundle("rat", "ratbundle.txt");
    load_resource_bundle("menu", "menubundle.txt");
    load_resource_bundle("blob", "blobbundle.txt");
    load_resource_bundle("fly", "flybundle.txt");
    load_resource_bundle("waterRat", "waterRatbundle.txt");
    load_resource_bundle("tentacle", "tentaclebundle.txt");

    open_window("Below The Surface", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    bool test_screen = false;
    bool window_border = false;
    int refresh_rate = 60;

    vector<string> cell_sheet_names;

    // Push the cell sheets used in the same order as the level editor
    cell_sheet_names.push_back("Solid"); //0
    cell_sheet_names.push_back("HalfBlocksTop"); //100
    cell_sheet_names.push_back("HalfBlocksBottom"); //200
    cell_sheet_names.push_back("Ladder"); //300
    cell_sheet_names.push_back("Pipe"); //400
    cell_sheet_names.push_back("Water"); //500
    cell_sheet_names.push_back("Toxic"); //600
    cell_sheet_names.push_back("HoldPipes"); //700
    cell_sheet_names.push_back("EmptyHold"); //800
    cell_sheet_names.push_back("TurnPipes"); //900
    cell_sheet_names.push_back("EmptyTurn"); //1000
    cell_sheet_names.push_back("Decorative"); //1100
    cell_sheet_names.push_back("Player"); //1200
    cell_sheet_names.push_back("Door"); //1300
    cell_sheet_names.push_back("Enemy"); //1400
    cell_sheet_names.push_back("MultiPipes"); //1500
    cell_sheet_names.push_back("EmptyMulti"); //1600
    cell_sheet_names.push_back("Collect"); //1700
    cell_sheet_names.push_back("Edge"); //1800

    // Timers
    create_timer("Dying");
    create_timer("DamageTimerP1");
    create_timer("DamageTimerP2");
    create_timer("DyingTimerP1");
    create_timer("DyingTimerP2");
    create_timer("SpawnTimerP1");
    create_timer("SpawnTimerP2");
    create_timer("DanceTime");
    create_timer("ScreenTimer");
    create_timer("BackstoryTimer");

    vector<CellSheet> cell_sheets = make_cell_sheets(cell_sheet_names);
    std::vector<std::string> args(argv, argv + argc);
    vector<string> files;

    try
    {
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (args[i] == "-l")
            {
                for (int j = 1; j < std::stoi(args[i + 1]) + 2; j++)
                    files.push_back(args[i + 1 + j]);
            }
            if (args[i] == "-t")
            {
                test_screen = true;
            }
            if (args[i] == "-b")
            {
                window_border = true;
            }
            if(args[i] == "-r")
            {
                refresh_rate = std::stoi(args[i + i]);
            }
        }
    }
    catch (const std::exception &e)
    {
        write_line(e.what());
        write_line("Closing program");
        exit(1);
    }
    
    if(!window_border)
        window_toggle_border("Below The Surface");

    shared_ptr<Screen> screen;
    if (test_screen)
    {
        // use this variable to test different screens
        shared_ptr<Screen> test_screen(new Screen(new CompanyIntroScreen, TILE_SIZE, cell_sheets, files));
        screen = test_screen;
    }
    else
    {
        shared_ptr<Screen> normal_screen(new Screen(new CompanyIntroScreen, TILE_SIZE, cell_sheets, files));
        screen = normal_screen;
    }

    while (!key_typed(ESCAPE_KEY) && !quit_requested())
    {
        screen->update();
        process_events();
        refresh_screen(refresh_rate);
    }

    free_resource_bundle("player");
    free_resource_bundle("game_resources");
    free_resource_bundle("roach");
    free_resource_bundle("snake");
    free_resource_bundle("rat");
    free_resource_bundle("menu");
    free_resource_bundle("blob");
    free_resource_bundle("fly");
    free_resource_bundle("waterRat");
    free_resource_bundle("tentacle");
    free_all_timers();
    return 0;
}
