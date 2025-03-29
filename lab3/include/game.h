#ifndef GAME_H
#define GAME_H

#include "castle.h"
#include "common.h"

// Game class is the main class of the game. It contains the game logic and the main loop.
class Game {
private:
    Castle castle;            // The castle object
    Castle initial_castle;    // Store the initial state of the castle
    int steps;                // The number of steps taken by the player

    // Helper functions
    
    // @brief Parse a command string and return the parsed result
    // @param cmd The command string to be parsed
    // @return The parsed result, packaged in a CommandParseResult struct
    CommandParseResult parse_command(const Command& cmd);

    // @brief Output the prompt information and get a command from the console
    // @return The command string entered by the player
    Command get_command();

    // @brief Move the player to the specified direction
    // @param direction The direction to move to
    // @return True if the move is successful, false otherwise
    bool move_player(Direction direction);

    // @brief Render the game screen
    void render();

    // @brief Return the current number of steps taken by the player
    int get_steps();

    // @brief Show the game message based on the current and previous game status
    // @param status The current game status
    // @param prev_status The previous game status
    void show_game_message(GameStatus status, GameStatus prev_status);

public:
    Game(int _width, int _height, int _level);

    // @brief Restart the current game
    void restart_game();

    // @brief Start the main game loop
    void mainloop();
};

#endif