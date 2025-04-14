#pragma once

#include "datafile_manager.h"
#include "diary.h"
#include "metadata.h"


class DiaryManager {
private:
    std::string file_path;
    DataFileManager data_file_manager;

    void init_data_file(const std::string& file_path);

public:
    DiaryManager(const std::string& file_path);

    void create_diary(const Diary& diary);

    int remove_diary(uint64_t data);

    const Diary& show_diary(uint64_t date);
    
    const std::vector<MetadataBasis>& select_diaries(uint64_t start_date, uint64_t end_date);
};

