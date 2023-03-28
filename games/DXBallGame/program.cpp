#include "splashkit.h"

int main()
{
    open_window("DX Ball Game", 800, 600);
    window_toggle_border("DX Ball Game");

    // Draw the environment

    while(!key_down(ESCAPE_KEY))
    {
        // Player controls

        // Calculate ball movement
        
        // Redraw everything
        clear_screen(COLOR_BLACK);

        refresh_screen(60);
    }
    return 0;
}