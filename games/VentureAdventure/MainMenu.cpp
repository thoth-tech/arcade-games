// MainMenu.cpp

#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu() {
    
}

void MainMenu::Render() {

    std::cout << "Venture Adventure Main Menu:\n";
    std::cout << "1. Play Game\n";
    std::cout << "2. Level Select\n";
    std::cout << "3. How to Play\n";
    std::cout << "4. Quit\n";
}
void MainMenu::ShowHowToPlay() {
    system("clear");
    cout << "=== How to Play ===\n";
    cout << "Welcome to Venture Adventure! In this game, your goal is to collect all the gems and complete each level.\n";
    cout << "Here are some tips to help you get started:\n";
    cout << "1. Movement:\n";
    cout << "   - Use the arrow keys (Up, Down, Left, Right) to move your character.\n";
    cout << "   - You can push boxes to clear paths and reach gems.\n";
    cout << "2. Collecting Gems:\n";
    cout << "   - Your objective is to collect all the gems on each level.\n";
    cout << "   - Gems are scattered throughout the level, and you must navigate obstacles to reach them.\n";
    cout << "3. Obstacles:\n";
    cout << "   - Beware of obstacles such as walls and pits; you cannot move through them.\n";
    cout << "   - Plan your moves carefully to avoid getting stuck.\n";
    cout << "4. Restarting Levels:\n";
    cout << "   - If you get stuck or want to restart a level, you can press 'R' to restart the current level.\n";
    cout << "5. Quitting the Game:\n";
    cout << "   - You can return to the main menu at any time by pressing 'Q' during gameplay.\n";
    cout << "6. Have Fun:\n";
    cout << "   - Most importantly, have fun and enjoy the adventure!\n";
    cout << "Press 'esc' to go back to the main menu.\n";
    cout << "====================\n";
}
