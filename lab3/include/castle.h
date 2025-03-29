#ifndef CASTLE_H
#define CASTLE_H

#include "common.h"
#include <random>

// Castle class is responsible for the following:
// 1. Randomly generate a castle with rooms
// 2. Randomly place contents (knight, princess, monster, exit, stair) in the rooms
// 3. Provide methods for interacting with the castle (move knight, check game status, etc.)
// 4. Render the castle to the console
class Castle {
private:
    int width, height, level;         // The shape of the castle
    Room *rooms;
    bool ready;                       // Whether the castle is ready to play

    // The positions of the exit, princess, monster, and knight in the castle
    Position exit_pos;
    Position princess_pos;
    Position monster_pos;
    Position knight_pos;

    // Random number generator
    std::mt19937 random_generator;

    // Helper functions

    // @brief Generate the map of the specified level in the castle
    // @param lvl The level to generate the map for
    void construct_level(int lvl);

    // @brief Get the string representation of the specified room content
    // @param content The one-hot encoding of the room content
    // @return The string representation of the room content
    std::string get_room_content_str(RoomContent content);

    // @brief Check if the specified position is valid in the castle
    // @param pos The position to check
    // @return Whether the position is valid in the castle
    bool check_position_valid(Position pos);

    // @brief Add an exit of a direction to the specified room
    // @param pos The position of the room to add the exit to
    // @param dir The direction of the exit to add
    void add_room_direction(Position pos, Direction dir);

    // @brief Add a content to the specified room
    // @param pos The position of the room to add the content to
    // @param content The one-hot encoding of the content to add to the room
    void add_room_content(Position pos, RoomContent content);

    // @brief Remove a content from the specified room
    // @param pos The position of the room to remove the content from
    // @param content The one-hot encoding of the content to remove from the room
    void remove_room_content(Position pos, RoomContent content);

    // @brief Move a content from one position to its neighbor in the specified direction
    // @param content The one-hot encoding of the content to move
    // @param src The position of the content to move
    // @param direction The direction to move the content in
    // @return Whether the content was successfully moved
    bool move_content(RoomContent content, Position src, Direction direction);

public:
    Castle();                                         // Default constructor
    Castle(int _width, int _height, int _level);      // Constructor with parameters
    Castle(const Castle &obj);                        // Copy constructor
    ~Castle();                                        // Destructor

    Castle& operator = (const Castle &obj);           // Assignment operator

    // @brief Move the knight in the specified direction
    // @param direction The direction to move the knight in
    // @return Whether the knight was successfully moved
    bool move_knight(Direction direction);

    // @brief Generate the castle map and place the contents in the rooms
    void generate_castle();

    // @brief Render the specified level of the castle to the console
    void render_castle(int lvl);

    // @brief Get the game status of the castle
    GameStatus get_game_status();

    // @brief Get the position of the knight in the castle
    Position get_knight_pos();

    // @brief Get the Room object at the specified position in the castle
    Room get_room(Position pos);
};

#endif