#include "diary_manager.h"
#include <sys/stat.h>

DiaryManager::DiaryManager(const std::string& file_path) {
    struct stat path_stat;
    if (stat(file_path.c_str(), &path_stat) != 0) {
        // file does not exist, create a new one

    }
}

void DiaryManager::init_data_file(const std::string& file_path) {
    // create a new data file
    
}
