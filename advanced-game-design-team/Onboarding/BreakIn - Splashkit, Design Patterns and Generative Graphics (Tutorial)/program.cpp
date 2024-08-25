#include "../../../games/splashkit mario/include/splashkit/splashkit.h"
#include "globals.h"
#include "types.h"
#include "state_init.h"
#include "state_management.h"
#include "draw.h"

int main()
{
    open_window("BreakIn", WINDOW_WIDTH, WINDOW_HEIGHT);
    hide_mouse();
    GameState game = new_game_state();
    while (!quit_requested())
    {
        process_events();
        if (mouse_clicked(LEFT_BUTTON)) {
            game.balls.push_back(
                    new_ball({static_cast<double>(rng.randomInt(GAME_AREA_START, GAME_AREA_END)),
                              static_cast<double>(GAME_AREA_HEIGHT - 100)}, {3, -3}, 3, clr_ball_standard)
            );
        }
        update_global_state(game);
        draw_global_state(game);
        refresh_screen(60);
    }
    close_window("BreakIn");
    return 0;
}
