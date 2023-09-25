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