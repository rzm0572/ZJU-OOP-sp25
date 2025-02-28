#ifndef STUDENT_H
#define STUDENT_H

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::array;
using std::cout;
using std::max;
using std::string;
using std::vector;

static const int NUM_SCORE = 3;   // number of scores per student
static const int BASE_WIDTH = 8;  // base width of each field in output

typedef array<int, NUM_SCORE> ScoreList;  // list of scores for a student

// struct to store statistics for scores of one course
struct StatPack {
    float avg_score;
    int min_score;
    int max_score;
    StatPack(float _avg = 0.0, int _min = 5, int _max = 0) : avg_score(_avg), min_score(_min), max_score(_max) {}
};

// class to store information of one student
class Record {
   private:
    int id;
    string name;
    ScoreList score;
    float avg_score;

    // @brief Calculate the average score of the student
    // @return the average score of the student
    float get_avg_score();

   public:

    Record() {
        id = 0;
        name = "";
        score = ScoreList({0});
        avg_score = 0.0;
    }

    Record(string _name, int _id = 0, ScoreList _score = ScoreList({0})) : id(_id), name(_name), score(_score) {
        avg_score = get_avg_score();
    }

    Record(string _name, int _id = 0, int _score0 = 0, int _score1 = 0, int _score2 = 0) : id(_id), name(_name) {
        score = {_score0, _score1, _score2};
        avg_score = get_avg_score();
    }

    ~Record() {}

    // @brief Read a line of input from a string and store the data in the record
    // @param line: a string containing the data of a student record in a line
    void read_string(string line, char delimiter = ',');

    // @brief Output the record in a formatted way in a line
    // @param max_name_length: the maximum length of the name field
    void output_inline(int max_name_length);

    // @brief Get the score of the student in a specified course
    // @param score_num: the id of the course whose score is to be returned (0-2)
    // @return the score of the specified course
    int get_score(int score_num);

    // @brief Get the length of the name field
    // @return the length of the name field
    int get_name_length();

    // @brief Set the id of the student
    // @param _id: the new id of the student
    void set_id(int _id);
};

/// Store a table of student records
class RecordTable {
   private:
    vector<Record> table;

    // @brief Get the maximum length of the name field in the table
    // @return the maximum length of the name field in the table
    int get_max_name_length();

   public:
    RecordTable() {
        table.clear();
    }

    ~RecordTable() {}

    // @brief Get some statistics of the scores in a specified course
    // @param score_num: the id of the course whose statistics are to be returned (0-2)
    // @return a StatPack object containing the average, minimum and maximum of the scores in the specified course
    StatPack get_stat(int score_num);

    // @brief Add a new student record to the table
    // @param student: a Record object representing the new student record to be added
    void add_line(Record student);

    // @brief Output the table header in a formatted way
    // @param max_name_length: the maximum length of the name field in the table
    void write_header(int max_name_length);

    // @brief Output the statistics of scores in a formatted way
    // @param max_name_length: the maximum length of the name field in the table
    void write_stat(int max_name_length);

    // @brief Output the entire table in a formatted way
    void write_all();
};

#endif