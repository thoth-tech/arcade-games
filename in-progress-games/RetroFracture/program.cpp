#include "splashkit.h"
#include "src/game.h"

/**
 * Main entry point for RetroFracture game
 * Sets up window for arcade machine compliance and launches the game
 */
int main() 
{
    // Initialize game window with arcade machine specifications
    open_window("RetroFracture", 800, 600);
    
    // Remove window border for arcade cabinet compliance
    window_toggle_border("RetroFracture");
    
    // Create and run the game instance
    Game game;
    game.run();
    
    return 0;
}