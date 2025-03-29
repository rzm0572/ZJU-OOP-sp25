#include <sstream>
#include "student.h"

// helper functions

void checkmin(int &x, int y) {
    if (y < x) x = y;
}

void checkmax(int &x, int y) {
    if (y > x) x = y;
}

// remove the leading and trailing whitespaces of a string
string strip(string s) {
    string res = s;
    res.erase(0, res.find_first_not_of(" "));
    res.erase(res.find_last_not_of(" ") + 1, string::npos);
    return res;
}

// member function of Record

float Record::get_avg_score() {
    float sum = 0;
    for (auto it = score.begin(); it != score.end(); ++it) {
        sum += *it;
    }
    return sum / NUM_SCORE;
}

void Record::read_string(string line, char delimiter) {
    std::stringstream ss(line);

    std::getline(ss, name, delimiter);
    name = strip(name);
    for (auto it = score.begin(); it != score.end(); ++it) {
        string score_str;
        std::getline(ss, score_str, delimiter);
        score_str = strip(score_str);
        *it = std::stoi(score_str);
    }
    avg_score = get_avg_score();
}

void Record::output_inline(int max_name_length) {
    // use cout.width() to format output
    cout.width(BASE_WIDTH);
    cout << std::left << id;

    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << name;

    for (auto it = score.begin(); it != score.end(); ++it) {
        cout.width(BASE_WIDTH);
        cout << std::left << *it;
    }

    cout.width(BASE_WIDTH);
    cout << std::left << avg_score << std::endl;
}

int Record::get_score(int score_num) {
    return score[score_num];
}

int Record::get_name_length() {
    return name.length();
}

void Record::set_id(int _id) {
    id = _id;
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

StatPack RecordTable::get_stat(int score_num) {
    StatPack pack;
    // traverse all records and calculate the statistics
    for (auto it = table.begin(); it != table.end(); ++it) {
        int cur_score = it->get_score(score_num);
        pack.avg_score += cur_score;
        checkmin(pack.min_score, cur_score);
        checkmax(pack.max_score, cur_score);
    }
    pack.avg_score /= (int)table.size();
    return pack;
}

void RecordTable::add_line(Record student) {
    student.set_id((int)table.size() + 1);
    table.push_back(student);
}

void RecordTable::write_header(int max_name_length) {
    // write header line
    cout.width(BASE_WIDTH);
    cout << std::left << "no";

    // use max() to make sure the name field is at least BASE_WIDTH wide
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "name";

    for (int i = 1; i <= NUM_SCORE; ++i) {
        cout.width(BASE_WIDTH);
        string score_header = "score" + std::to_string(i);
        cout << std::left << score_header;
    }

    cout.width(BASE_WIDTH);
    cout << std::left << "average" << std::endl;
}

void RecordTable::write_stat(int max_name_length) {
    StatPack pack[NUM_SCORE];
    for (int i = 0; i < NUM_SCORE; i++) {
        pack[i] = get_stat(i);
    }

    // write average score of each course
    cout.width(BASE_WIDTH);
    cout << std::left << "";
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "average";

    for (int i = 0; i < NUM_SCORE; i++) {
        cout.width(BASE_WIDTH);
        cout << std::left << pack[i].avg_score;
    }
    cout << std::endl;

    // write min score of each course
    cout.width(BASE_WIDTH);
    cout << std::left << "";
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "min";

    for (int i = 0; i < NUM_SCORE; i++) {
        cout.width(BASE_WIDTH);
        cout << std::left << pack[i].min_score;
    }
    cout << std::endl;

    // write max score of each course
    cout.width(BASE_WIDTH);
    cout << std::left << "";
    cout.width(max(max_name_length + 1, BASE_WIDTH));
    cout << std::left << "max";

    for (int i = 0; i < NUM_SCORE; i++) {
        cout.width(BASE_WIDTH);
        cout << std::left << pack[i].max_score;
    }
    cout << std::endl;
}

void RecordTable::write_all() {
    int max_name_length = get_max_name_length();
    write_header(max_name_length);            // write header
    for (auto it = table.begin(); it != table.end(); ++it) {
        it->output_inline(max_name_length);   // write each record
    }
    write_stat(max_name_length);              // write statistics
}
