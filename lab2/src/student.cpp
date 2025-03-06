#include <sstream>
#include "student.h"

// Helper macros

// If condition is true, output output_1, otherwise output output_2
#define CONDITIONAL_OUTPUT(condition, output_1, output_2) \
{                                                         \
    if (condition) {                                      \
        cout << std::left << output_1;                    \
    } else {                                              \
        cout << std::left << output_2;                    \
    }                                                     \
}

// Helper functions

// Update x if y is smaller than x
void checkmin(int &x, int y) {
    if (y < x) x = y;
}

// Update x if y is larger than x
void checkmax(int &x, int y) {
    if (y > x) x = y;
}

// @brief Remove the leading and trailing whitespaces of a string
string strip(string s) {
    string res = s;
    if (res.length() > 0) res.erase(0, res.find_first_not_of(" "));
    if (res.length() > 0) res.erase(res.find_last_not_of(" ") + 1, string::npos);
    return res;
}

// member function of Record

float Record::get_avg_score() {
    float sum = 0;
    int chosen_courses = 0;
    for (auto it = score.begin(); it != score.end(); ++it) {
        sum += it->second;
        chosen_courses++;
    }

    // If the student has not chosen any course, return NO_CHOSEN_COURSE
    if (chosen_courses == 0) {
        return NO_CHOSEN_COURSE;
    } else {
        return sum / chosen_courses;
    }
}

void Record::read_string(string line, char delimiter) {
    // Use stringstream to parse the input string
    std::stringstream ss(line);

    // Read the name
    std::getline(ss, name, delimiter);
    name = strip(name);

    // Read the course-score pairs
    string course_score_str;
    while (std::getline(ss, course_score_str, delimiter)) {
        std::stringstream course_score_ss(course_score_str);
        string course_name, score_str;
        
        // Read the course name, skip post processing if empty
        if (!std::getline(course_score_ss, course_name, ':')) {
            continue;
        }

        std::getline(course_score_ss, score_str);   // Read the score
        
        course_name = strip(course_name);
        score_str = strip(score_str);
        if (score_str.length() > 0) {    // If the score is not empty, add it to the score list
            score[course_name] = std::stoi(score_str);
        }
    }
    avg_score = get_avg_score();         // update the average score
}

void Record::output_inline(FieldWidth max_name_length, CourseList exist_courses) {
    // Use cout.width() to format output
    cout.width(BASE_WIDTH);
    cout << std::left << id;

    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << name;

    // Check if the course is chosen by the student, output N/A if not
    for (auto it = exist_courses.begin(); it != exist_courses.end(); ++it) {
        cout.width(max(it->length() + 1, BASE_WIDTH));
        CONDITIONAL_OUTPUT(score.find(*it) == score.end(), "N/A", score[*it]);
    }

    cout.width(BASE_WIDTH);

    // Output the average score, output N/A if the student has not chosen any course
    CONDITIONAL_OUTPUT(avg_score < 0, "N/A" << std::endl, avg_score << std::endl);
}

int Record::get_score(string course_name) {
    if (score.find(course_name) == score.end()) {
        return COURSE_NOT_FOUND;
    } else {
        return score[course_name];
    }
}

int Record::get_name_length() {
    return name.length();
}

void Record::set_id(int _id) {
    id = _id;
}

int Record::get_course_num() {
    return score.size();
}

CourseList Record::get_course_list() {
    CourseList course_list;
    for (auto it = score.begin(); it != score.end(); ++it) {
        course_list.insert(it->first);
    }
    return course_list;
}

// member function of RecordTable

int RecordTable::get_max_name_length() {
    int max_name_length = 0;
    // calculate the maximum length of name in the table, used for output formatting
    for (auto it = table.begin(); it != table.end(); ++it) {
        checkmax(max_name_length, it->get_name_length());
    }
    return max_name_length;
}

StatPack RecordTable::get_stat(string course_name) {
    StatPack pack;
    pack.chosen_students = 0;
    pack.width = max(course_name.length() + 1, BASE_WIDTH);

    // traverse all records and calculate the statistics
    for (auto it = table.begin(); it != table.end(); ++it) {
        int cur_score = it->get_score(course_name);
        // If the course is not chosen by the student, skip the processing
        if (cur_score != COURSE_NOT_FOUND) {
            pack.avg_score += cur_score;
            checkmin(pack.min_score, cur_score);
            checkmax(pack.max_score, cur_score);
            pack.chosen_students++;
        }
    }
    
    // If the course is not chosen by any student, set the average score to NO_CHOSEN_STUDENT
    if (pack.chosen_students == 0) {
        pack.avg_score = NO_CHOSEN_STUDENT;
        pack.min_score = NO_CHOSEN_STUDENT;
        pack.max_score = NO_CHOSEN_STUDENT;
    } else {  // If the course is chosen by some students, calculate the average score
        pack.avg_score /= pack.chosen_students;
    }
    return pack;
}

void RecordTable::add_line(Record student) {
    student.set_id((int)table.size() + 1);     // set the id of the student
    table.push_back(student);                  // add the student to the table

    // update the course list
    CourseList course_list = student.get_course_list();
    courses.insert(course_list.begin(), course_list.end());

    // If a new record is added, the statpack is not updated
    pack_updated = false;
}

void RecordTable::add_line(string line, char delimiter) {
    Record student;
    student.set_id((int)table.size() + 1);     // set the id of the student
    student.read_string(line, delimiter);      // parse the input string
    table.push_back(student);                  // add the student to the table

    // update the course list
    CourseList course_list = student.get_course_list();
    courses.insert(course_list.begin(), course_list.end());

    // If a new record is added, the statpack is not updated
    pack_updated = false;
}

void RecordTable::write_header(FieldWidth max_name_length) {
    // write header line
    cout.width(BASE_WIDTH);
    cout << std::left << "no";

    // use max() to make sure the name field is at least BASE_WIDTH wide
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "name";

    for (auto it = courses.begin(); it != courses.end(); ++it) {
        cout.width(max(it->length(), BASE_WIDTH));
        cout << std::left << *it;
    }

    cout.width(BASE_WIDTH);
    cout << std::left << "average" << std::endl;
}

void RecordTable::write_stat(FieldWidth max_name_length) {
    // If the pack is not updated, update it first
    if (!pack_updated) {
        for (auto it = courses.begin(); it != courses.end(); ++it) {
            pack.push_back(get_stat(*it));
        }
        pack_updated = true;
    }

    // write average score of each course
    cout.width(BASE_WIDTH);
    cout << std::left << "";
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "average";

    for (auto it = pack.begin(); it != pack.end(); ++it) {
        cout.width(it->width);
        CONDITIONAL_OUTPUT(it->chosen_students == 0, "N/A", it->avg_score);
    }
    cout << std::endl;

    // write min score of each course
    cout.width(BASE_WIDTH);
    cout << std::left << "";
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "min";

    for (auto it = pack.begin(); it != pack.end(); ++it) {
        cout.width(it->width);
        CONDITIONAL_OUTPUT(it->chosen_students == 0, "N/A", it->min_score);
    }
    cout << std::endl;

    // write max score of each course
    cout.width(BASE_WIDTH);
    cout << std::left << "";
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "max";

    for (auto it = pack.begin(); it != pack.end(); ++it) {
        cout.width(it->width);
        CONDITIONAL_OUTPUT(it->chosen_students == 0, "N/A", it->max_score);
    }
    cout << std::endl;
}

void RecordTable::write_all() {
    int max_name_length = get_max_name_length();
    write_header(max_name_length);                     // write header
    for (auto it = table.begin(); it != table.end(); ++it) {
        it->output_inline(max_name_length, courses);   // write each record
    }
    write_stat(max_name_length);                       // write statistics
}
