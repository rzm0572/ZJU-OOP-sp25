#include <iostream>
#include "game.h"

int main() {
    int width, height, level;
    std::cout << "Welcome to the game!" << std::endl;

    // Get castle shape from user
    std::cout << "Enter castle size (format: width height level): ";
    std::cin >> height >> width >> level;
    std::cin.get();

    // Create game object and start game loop
    Game game(width, height, level);
    game.mainloop();
    return 0;
}
