#include "game.h"
#include "common.h"
#include <sstream>
#include <iostream>

// @brief Get the name of a room based on its attributes
std::string get_room_name(const Room& room) {
    // If the room is an exit, the name is "lobby"
    if (room.content & EXIT_BIT) {
        return "lobby";
    }

    if (room.type & ~0x0F) {
        // If the room has stairs, the name is "stairwells"
        return "stairwells";
    } else {
        // Otherwise, check the room_info table for the name
        return room_info[room.type].room_name;
    }
}

Game::Game(int _width, int _height, int _level) {
    // Create a new castle with the given dimensions and level
    castle = Castle(_width, _height, _level);

    // Generate the castle map
    castle.generate_castle();

    // Save the initial state of the castle for restarting the game
    initial_castle = castle;
    steps = 0;                  // The player starts with 0 steps
}

void Game::restart_game() {
    castle = initial_castle;    // Reset the castle to the initial state
    steps = 0;                  // Reset the number of steps taken
}

CommandParseResult Game::parse_command(const Command& cmd) {
    // Use a stringstream to parse the input command
    std::stringstream ss(cmd);
    std::string op_type, direction_str;

    ss >> op_type;
    if (op_type == "go") {       // Move command
        ss >> direction_str;     // Get the direction
        for (int i = 0; i < NUM_DIRECTIONS; i++) {    // Check the direction_info table for the type of direction
            if (direction_str == direction_info[i].name) {
                return CommandParseResult(MOVE_CMD, direction_info[i].direction);
            }
        }
    } else if (op_type == "quit" || op_type == "exit") {  // Quit command
        return CommandParseResult(QUIT_CMD);
    } else if (op_type == "restart") {                    // Restart command
        return CommandParseResult(RESTART_CMD);
    }

    return CommandParseResult(ERROR_CMD);            // Invalid command
}

Command Game::get_command() {
    // Get the player's current position and room
    Position player_pos = castle.get_knight_pos();
    Room player_room = castle.get_room(player_pos);

    // Check the exits in the current room and get the valid directions
    int num_exit = 0;
    std::vector<std::string> valid_directions;
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (player_room.type & direction_info[i].bit_mask) {
            num_exit++;
            valid_directions.push_back(direction_info[i].name);
        }
    }

    // Output the prompt
    std::cout << "Welcome to the " << get_room_name(player_room) << ". There are " << num_exit << " exits";
    if (num_exit >= 1) {
        std::cout << ": " << valid_directions[0];
    }
    if (num_exit >= 2) {
        for (int i = 1; i < num_exit - 1; i++) {
            std::cout << ", " << valid_directions[i];
        }
        std::cout << " and " << valid_directions[num_exit - 1];
    }
    std::cout << "." << std::endl << "Enter your command: ";

    // Get the player's input command
    Command cmd;
    std::getline(std::cin, cmd);
    return cmd;
}

bool Game::move_player(Direction direction) {
    // Call the castle's move_knight function to move the knight
    return castle.move_knight(direction);
}

int Game::get_steps() {
    return steps;
}

void Game::show_game_message(GameStatus status, GameStatus prev_status = PREPARE_S) {
    if (status == WIN_S) {                                               // Prompt when the player wins the game
        std::cout << "Congratulations! You win!" << std::endl;
    } else if (status == LOSE_S) {                                       // Prompt when the player loses the game
        std::cout << "Sorry, you lose!" << std::endl;
    } else if (status == QUIT_S) {                                       // Prompt when the player quits the game by command
        std::cout << "Goodbye!" << std::endl;
    } else if (status == RUNNING2_S && prev_status == RUNNING1_S) {      // Prompt when the knight and princess meet
        std::cout << "Knight: I come to save you!" << std::endl;
        std::cout << "Princess: Thank you, brave knight!" << std::endl;
        std::cout << "Knight: My pleasure. Let's return to the entrance." << std::endl;
    }
}

void Game::render() {
    system("clear");                                      // Clear the screen
    Position player_pos = castle.get_knight_pos();        // Get the player's position
    castle.render_castle(player_pos.level);          // Render the level where the player is
    std::cout << "Steps: " << steps << std::endl;         // Show the number of steps taken
}

void Game::mainloop() {
    GameStatus prev_status = castle.get_game_status();

    render();
    while (true) {
        // Get the player's input command and parse it
        Command command = get_command();
        CommandParseResult cmd_parse_result = parse_command(command);

        // If the command is invalid, show an error message and continue
        if (cmd_parse_result.type == ERROR_CMD) {
            std::cout << "Invalid command. Please try again." << std::endl;
            continue;
        }

        // If the command is quit or restart, handle it and break out of the loop
        if (cmd_parse_result.type == QUIT_CMD) {
            show_game_message(QUIT_S);
            break;
        }

        // If the command is restart, restart the game and continue the loop
        if (cmd_parse_result.type == RESTART_CMD) {
            restart_game();
            render();
            continue;
        }

        // If the command is move, move the player and update the number of steps taken
        Direction direction = cmd_parse_result.direction;

        if (direction == ERROR_D) {
            continue;
        }

        if (move_player(direction)) {
            steps++;
        }

        // Render the game and check the game status
        render();
        GameStatus status = castle.get_game_status();
        
        // Show the appropriate message based on the game status
        show_game_message(status, prev_status);

        // If the game is over, break out of the loop
        if (status == WIN_S || status == LOSE_S) {
            break;
        }

        // Update the previous game status
        prev_status = status;
    }
}
