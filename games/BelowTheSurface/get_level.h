#include "splashkit.h"
#include "level.h"
#include <memory>

shared_ptr<Level> get_next_level(int level, vector<CellSheet> cell_sheets, int tile_size, int players)
{ 
    shared_ptr<Level> next_level;
    switch(level)
    {
        
        case 1:
            {
                shared_ptr<Level> level0(new Level0(cell_sheets, tile_size, players));
                next_level = level0;
                break;
            }
        case 2:
            {
                shared_ptr<Level> multi(new MultiPipe(cell_sheets, tile_size, players));
                next_level = multi;
                break;
            }
        case 3:
            {
                shared_ptr<Level> easy(new Easy(cell_sheets, tile_size, players));
                next_level = easy;
                break;
            }
        
        case 4:
            {
                shared_ptr<Level> level3(new Level3(cell_sheets, tile_size, players));
                next_level = level3;
                break;
            }

        case 5:
            {
                shared_ptr<Level> level6(new Level6(cell_sheets, tile_size, players));
                next_level = level6;
                break;
            }
        case 6:
            {
                shared_ptr<Level> trials(new FourCorners(cell_sheets, tile_size, players));
                next_level = trials;
                break;
            }
        case 7:
            {
                shared_ptr<Level> rats(new Rats(cell_sheets, tile_size, players));
                next_level = rats;
                break;
            }
        case 8:
            {
                shared_ptr<Level> surfin(new Surf(cell_sheets, tile_size, players));
                next_level = surfin;
                break;
            }
        case 9:
            {
                shared_ptr<Level> combat(new CombatLevel(cell_sheets, tile_size, players));
                next_level = combat;
                break;
            }
        case 10:
            {
                shared_ptr<Level> boss(new BossLevel(cell_sheets, tile_size, players));
                next_level = boss;
                break;
            }
        case 40:
            {
                shared_ptr<Level> roach(new TooManyRoach(cell_sheets, tile_size, players));
                next_level = roach;
                break;
            }
        case 50:
            {
                shared_ptr<Level> mario(new Mario(cell_sheets, tile_size, players));
                next_level = mario;
                break;
            }
        default:
            {
                shared_ptr<Level> def(new TooManyRoach(cell_sheets, tile_size, players));
                next_level = def;
                break;
            }
    }

    return next_level;
}