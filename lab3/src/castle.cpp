#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <ctime>
#include "castle.h"
#include "common.h"
#include "disjoint_set.h"

enum EdgeType { EAST_WEST, NORTH_SOUTH };

const std::string ContentChar = "KPMES";

struct Edge {
    int u;
    int v;
    int f;
    EdgeType type;
    Edge(int _u = 0, int _v = 0, int _f = 0, EdgeType _type = EAST_WEST): u(_u), v(_v), f(_f), type(_type) {}
    bool operator < (const Edge& e) const {
        return f < e.f;
    }
};

Castle::Castle() {}

Castle::Castle(int _width, int _height, int _level): width(_width), height(_height), level(_level) {
    ready = false;  // The castle is not ready to play

    // Initialize the rooms
    rooms = new Room[_width * _height * _level];
    for (int i = 0; i < _level; i++) {
        for (int j = 0; j < _height; j++) {
            for (int k = 0; k < _width; k++) {
                rooms[i * _width * _height + j * _width + k].pos = Position(k, j, i);
            }
        }
    }
}

Castle::Castle(const Castle& obj): width(obj.width), height(obj.height), level(obj.level) {
    ready = false;   // The castle is not ready to play

    // Copy the rooms from the source object
    rooms = new Room[width * height * level];
    for (int i = 0; i < width * height * level; i++) {
        rooms[i] = obj.rooms[i];
    }
    ready = true;    // mark the completion of the map generation
}

Castle::~Castle() {
    delete[] rooms;
}

Castle& Castle::operator = (const Castle& obj) {
    ready = false;    // The castle is not ready to play

    // Copy the rooms from the source object
    width = obj.width;
    height = obj.height;
    level = obj.level;
    exit_pos = obj.exit_pos;
    princess_pos = obj.princess_pos;
    monster_pos = obj.monster_pos;
    knight_pos = obj.knight_pos;

    rooms = new Room[width * height * level];
    for (int i = 0; i < width * height * level; i++) {
        rooms[i] = obj.rooms[i];
    }
    ready = true;      // mark the completion of the map generation
    return *this;
}

bool Castle::check_position_valid(Position pos) {
    return pos.x >= 0 && pos.x < height && pos.y >= 0 && pos.y < width && pos.level >= 0 && pos.level < level;
}

void Castle::add_room_direction(Position pos, Direction dir) {
    if (check_position_valid(pos)) {
        rooms[pos.level * width * height + pos.x * width + pos.y].add_direction(dir);
    }
}

void Castle::add_room_content(Position pos, RoomContent new_content) {
    if (check_position_valid(pos)) {
        rooms[pos.level * width * height + pos.x * width + pos.y].add_content(new_content);
    }
}

void Castle::remove_room_content(Position pos, RoomContent removed_content) {
    if (check_position_valid(pos)) {
        rooms[pos.level * width * height + pos.x * width + pos.y].remove_content(removed_content);
    }
}

bool Castle::move_knight(Direction direction) {
    RoomContent content = KNIGHT_BIT;
    // If the knight has met the princess, move them together
    if (knight_pos == princess_pos) {
        content |= PRINCESS_BIT;
    }
    return move_content(content, knight_pos, direction);
}

bool Castle::move_content(RoomContent content, Position src, Direction direction) {
    // Check whether source position is valid
    if (!check_position_valid(src)) {
        return false;
    }

    // Check whether the movement is valid
    if (!rooms[src.level * width * height + src.x * width + src.y].has_direction(direction)) {
        return false;
    }
    
    // Check whether the destination position is valid
    Position dst = src.neighbor(direction);
    if (!check_position_valid(dst)) {
        return false;
    }

    remove_room_content(src, content);  // Remove the content from the source position
    add_room_content(dst, content);         // Add the content to the destination position
    
    // Update the position of the knight and princess
    if (content & KNIGHT_BIT) {
        knight_pos.go_neighbor(direction);
    }
    if (content & PRINCESS_BIT) {
        princess_pos.go_neighbor(direction);
    }

    return true;
}

void Castle::construct_level(int lvl) {
    // Use vertics to represent the rooms
    // Use edges to represent the walls between rooms
    std::vector<Edge> edges;
    
    // Initialize the random generator
    random_generator.seed(time(NULL));
    std::uniform_int_distribution<uint32_t> uint_dist(0, width * height * 4);

    // Randomly set weight to each wall between rooms
    // Vertical walls
    for (int i = 0; i < width - 1; i++) {
        for (int j = 0; j < height; j++) {
            edges.push_back(Edge(j * width + i, j * width + i + 1, uint_dist(random_generator), EAST_WEST));
        }
    }

    // Horizontal walls
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width; j++) {
            edges.push_back(Edge(i * width + j, (i + 1) * width + j, uint_dist(random_generator), NORTH_SOUTH));
        }
    }

    // Sort the edges by their weight
    std::sort(edges.begin(), edges.end());

    // Use prim's algorithm to find the minimum spanning tree of the graph, which forms a connected maze map
    DisjointSet dset(width * height);                // Initialize the disjoint set with the number of vertices
    Room* rooms_lvl = rooms + lvl * width * height;
    for (auto e = edges.begin(); e!= edges.end(); e++) {
        if (!dset.is_in_same_set(e->u, e->v)) {
            // Break the wall if the edge is in the minimum spanning tree
            if (e->type == EAST_WEST) {
                rooms_lvl[e->u].add_direction(EAST_D);
                rooms_lvl[e->v].add_direction(WEST_D);
            } else if (e->type == NORTH_SOUTH) {
                rooms_lvl[e->u].add_direction(SOUTH_D);
                rooms_lvl[e->v].add_direction(NORTH_D);
            }
            dset.merge(e->u, e->v);
        }
    }
}

void Castle::generate_castle() {
    // Generate the map of each level
    for (int lvl = 0; lvl < level; lvl++) {
        construct_level(lvl);
    }

    // Randomly place the stairs
    // Initialize the random generator
    random_generator.seed(time(NULL));
    std::uniform_int_distribution<uint32_t> width_dist(0, width - 1);
    std::uniform_int_distribution<uint32_t> height_dist(0, height - 1);
    std::uniform_int_distribution<uint32_t> lvl_dist(0, level - 1);
    std::uniform_int_distribution<uint32_t> num_stairs_dist(1, 3);

    for (int lvl = 0; lvl < level - 1; lvl++) {
        int num_stairs = num_stairs_dist(random_generator);
        std::vector<Position> stairs_pos;
        while (stairs_pos.size() < num_stairs) {
            Position new_pos(width_dist(random_generator), height_dist(random_generator), lvl);
            
            bool unique = true;
            for (auto pos = stairs_pos.begin(); pos != stairs_pos.end(); pos++) {
                if (new_pos == *pos) {
                    unique = false;
                }
            }
            if (unique) {
                stairs_pos.push_back(new_pos);
            }
        }

        for (auto pos = stairs_pos.begin(); pos != stairs_pos.end(); pos++) {
            Position upstair = pos->neighbor(UP_D);
            add_room_direction(*pos, UP_D);
            add_room_direction(upstair, DOWN_D);
            add_room_content(*pos, STAIR_BIT);
            add_room_content(upstair, STAIR_BIT);
        }
    }

    // Randomly set the initial position of the knight, princess and monster, and the exit position
    exit_pos = Position(height_dist(random_generator), width_dist(random_generator), 0);
    princess_pos = Position(height_dist(random_generator), width_dist(random_generator), lvl_dist(random_generator));
    monster_pos = Position(height_dist(random_generator), width_dist(random_generator), lvl_dist(random_generator));
    knight_pos = exit_pos;

    // Add the content to the map
    add_room_content(exit_pos, EXIT_BIT);
    add_room_content(princess_pos, PRINCESS_BIT);
    add_room_content(monster_pos, MONSTER_BIT);
    add_room_content(knight_pos, KNIGHT_BIT);

    ready = true;   // mark the completion of the map generation
}

std::string Castle::get_room_content_str(RoomContent content) {
    int num_content_bits = ContentChar.length();
    std::string raw_content_str = "";
    // Convert the one-hot encoding of content to a string of characters
    for (int i = 0; i < num_content_bits; i++) {
        if (content & (1 << i)) {
            raw_content_str += ContentChar[i];
        }
    }

    // Pad the string with spaces if it is too short or too long
    std::string content_str = "";
    switch (raw_content_str.length()) {
        case 0: content_str = "   "; break;
        case 1: content_str = " " + raw_content_str + " "; break;
        case 2: content_str = raw_content_str.insert(1, " "); break;
        default: content_str = raw_content_str.substr(0, 3); break;
    }
    return content_str;
}

void Castle::render_castle(int lvl) {
    Room* rooms_lvl = rooms + lvl * width * height;

    // Render the upper border of the castle
    std::cout << "┌";
    for (int i = 0; i < width - 1; i++) {
        std::cout << "───┬";
    }
    std::cout << "───┐" << std::endl;

    for (int i = 0; i < height; i++) {
        // Render the contents and the vertical walls of the row
        std::cout << "│";        // Render the left border of the row
        for (int j = 0; j < width; j++) {
            // Render the content of the room
            Room* room_ij = rooms_lvl + i * width + j;
            std::cout << get_room_content_str(room_ij->content);

            // Check if the room is connected to its east neighbor
            if (room_ij->has_direction(EAST_D)) {
                std::cout << " ";
            } else {
                std::cout << "│";
            }
        }
        std::cout << std::endl;

        // Render the horizontal walls of the row
        if (i == height - 1) {
            std::cout << "└";    // Render the bottom-left corner of the castle
        } else {
            std::cout << "├";    // Render the left border
        }

        for (int j = 0; j < width; j++) {
            // Check if the room is connected to its south neighbor
            Room* room_ij = rooms_lvl + i * width + j;
            if (room_ij->has_direction(SOUTH_D)) {
                std::cout << "   ";
            } else {
                std::cout << "───";
            }
            
            // Render the right border of the castle
            if (j == width - 1) {
                if (i == height - 1) {
                    std::cout << "┘";    // Render the bottom-right corner of the castle
                } else {
                    std::cout << "┤";    // Render the right border
                }
            } else {
                if (i == height - 1) {
                    std::cout << "┴";    // Render the bottom border
                } else {
                    std::cout << "┼";    // Render the intersection of the walls
                }
            }
        }
        std::cout << std::endl;
    }
}

GameStatus Castle::get_game_status() {
    // Get the game status based on the position of content and the status of the castle map
    if (!ready) {
        return PREPARE_S;                 // If the castle map has not been generated, it is in the prepare status
    }
    if (knight_pos == princess_pos && knight_pos == exit_pos) {
        return WIN_S;                     // If the knight has met the princess and led her to the exit, it is in the win status
    }
    if (knight_pos == monster_pos) {
        return LOSE_S;                    // If the knight meets the monster, it is in the lose status
    }
    if (knight_pos == princess_pos) {
        return RUNNING2_S;                // If the knight has met the princess, it is in the running2 status
    }
    return RUNNING1_S;                    // If the knight hasn't met the princess, it is in the running1 status
}

Position Castle::get_knight_pos() {
    return knight_pos;
}

Room Castle::get_room(Position pos) {
    return Room(rooms[pos.level * width * height + pos.x * width + pos.y]);
}

