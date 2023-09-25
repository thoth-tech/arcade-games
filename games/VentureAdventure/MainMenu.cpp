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
    std::cout << "=== How to Play ===\n";
    std::cout << "Collect all the gems to win the game.\n";
    std::cout << "You can move the boxes to reach the gems.\n";
    std::cout << "Press 'esc' to go back to the main menu.\n";
    std::cout << "====================\n";
}
