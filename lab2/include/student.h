#ifndef STUDENT_H
#define STUDENT_H

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <initializer_list>

#define NO_CHOSEN_COURSE -1
#define NO_CHOSEN_STUDENT -1
#define COURSE_NOT_FOUND -1

using std::cout;
using std::max;
using std::string;
using std::vector;
using std::set;
using std::map;

typedef set<string> CourseList;  // list of course names
typedef map<string, int> ScoreList;  // list of scores for a student
typedef unsigned long FieldWidth;

static const FieldWidth BASE_WIDTH = 8;  // base width of each field in output

// struct to store statistics for scores of one course
struct StatPack {
    float avg_score;          // the average score of the course
    int min_score;            // the minimum score of the course
    int max_score;            // the maximum score of the course
    int chosen_students;      // the number of students who chose this course
    FieldWidth width;         // output width of the field
    StatPack(float _avg = 0.0, int _min = 5, int _max = 0, int _chosen = 0, FieldWidth _width = BASE_WIDTH) : avg_score(_avg), min_score(_min), max_score(_max), chosen_students(_chosen), width(_width) {}
};

// class to store information of one student
class Record {
   private:
    int id;               // the id of the student
    string name;          // the name of the student
    ScoreList score;      // the scores of the student in different courses
    float avg_score;      // the average score of the student

    // @brief Calculate the average score of the student
    // @return the average score of the student
    float get_avg_score();

   public:

    // @brief Default constructor
    Record() {
        id = 0;
        name = "";
        score = ScoreList();
        avg_score = 0.0;
    }

    // @brief Constructor with parameters
    Record(string _name, int _id = 0, ScoreList _score = ScoreList()) : id(_id), name(_name), score(_score) {
        avg_score = get_avg_score();
    }

    // @brief Constructor with initializer list
    Record(string _name, int _id = 0, std::initializer_list<std::pair<const string, int>> _score = {}) : id(_id), name(_name) {
        score.insert(_score);
        avg_score = get_avg_score();
    }

    // @brief Destructor
    ~Record() {}

    // @brief Read a line of input from a string and store the data in the record
    // @param line: a string containing the data of a student record in a line
    // @param delimiter: delimiter between scores of two courses
    void read_string(string line, char delimiter = ',');

    // @brief Output the record in a formatted way in a line
    // @param max_name_length: the maximum length of the name field
    void output_inline(FieldWidth max_name_length, CourseList exist_courses = CourseList());

    // @brief Get the score of the student in a specified course
    // @param course_name: the name of course
    // @return the score of the specified course
    int get_score(string course_name);

    // @brief Get the length of the name field
    // @return the length of the name field
    int get_name_length();

    // @brief Set the id of the student
    // @param _id: the new id of the student
    void set_id(int _id);

    // @brief Get the number of all available courses
    // @return the number of all available courses
    int get_course_num();

    // @brief Get the set of courses chosen by this student
    // @return a CourseList contains the courses chosen by this student
    CourseList get_course_list();
};

/// Store a table of student records
class RecordTable {
   private:
    vector<Record> table;         // The table of student score records
    CourseList courses;           // The set of all available courses
    vector<StatPack> pack;
    bool pack_updated;

    // @brief Get the maximum length of the name field in the table
    // @return the maximum length of the name field in the table
    int get_max_name_length();

   public:
    // @brief Default constructor
    RecordTable() {
        table.clear();
        courses.clear();
        pack.clear();
        pack_updated = false;
    }

    // @brief Destructor
    ~RecordTable() {}

    // @brief Get some statistics of the scores in a specified course
    // @param course_name: the name of course
    // @return a StatPack object containing the average, minimum and maximum of the scores in the specified course
    StatPack get_stat(string course_name);

    // @brief Add a new student record to the table
    // @param student: a Record object representing the new student record to be added
    void add_line(Record student);

    // @brief Parse the input string and add a new student record to the table
    // @param line: a string containing the data of a student record in a line
    // @param delimiter: delimiter between scores of two courses
    void add_line(string line, char delimiter = ',');

    // @brief Output the table header in a formatted way
    // @param max_name_length: the maximum length of the name field in the table
    void write_header(FieldWidth max_name_length);

    // @brief Output the statistics of scores in a formatted way
    // @param max_name_length: the maximum length of the name field in the table
    void write_stat(FieldWidth max_name_length);

    // @brief Output the entire table in a formatted way
    void write_all();
};

#endif