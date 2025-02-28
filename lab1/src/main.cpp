#include <iostream>

#include "student.h"

using std::cin;
using std::cout;

int main() {
    RecordTable table;
    int num_students = 0;
    cin >> num_students;
    cin.get();

    for (int i = 0; i < num_students; i++) {
        string record_str;
        std::getline(cin, record_str);

        Record record;
        record.read_string(record_str, ',');
        
        table.add_line(record);
    }

    table.write_all();
    return 0;
}
