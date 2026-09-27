#include "define.h"
#include "terminal.h"
#include "utils.h"
#include "draw.h"
#include "control.h"
#include "game.h"
#include "window.h"

void init()
{

    ui::window_init();
    ui::show_windows();
    ui::show_help();
    gm::init();
    gm::start_listener();
}

void loop()
{
    while (gm::running)
    {
        gm::process();
        ui::show_info();
        ui::show_game();
        ui::window_resize();
        std::this_thread::sleep_for(10ms);
    }
}

void exit()
{
    ui::show_exit();
    ui::window_exit();
}

int main()
{
    init();
    loop();
    exit();

    return 0;
}