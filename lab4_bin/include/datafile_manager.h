#pragma once

#include "page_table.h"
#include "buffer.h"
#include <cstring>
#include <string>
#include <fstream>
#include <cstdint>

const uint64_t HEADER_PAGE = 0;
const uint64_t FILE_TYPE_LENGTH = 8;
const std::string FILE_TYPE = "PDIARY";

struct Header {
    char file_type[FILE_TYPE_LENGTH];
    // uint64_t num_pages;
    // uint64_t begin_page_table;
    uint64_t size_page_table;
    // uint64_t begin_metadata;
    PageIndexList page_table_index;
    
    Header(std::string file_type = "", uint64_t size_page_table = 0, PageIndexList page_table_index = {}): size_page_table(size_page_table), page_table_index(page_table_index) {
        std::strncpy(this->file_type, file_type.c_str(), FILE_TYPE_LENGTH);
    };

    bool check_file_identifier() {
        return std::string(file_type) == FILE_TYPE;
    }

    bool check_valid_structure() {
        return size_page_table > 0 && page_table_index.size() > 0;
    }
};




class DataFileManager {
private:
    std::string file_path;
    std::ifstream input_fs;
    std::ofstream output_fs;
    Buffer buffer;

    Header header;
    PageTable page_table;
    // MetadataList metadata_list;

    // template<typename T>
    // T extract_data(uint64_t& pos);

    // template<typename T>
    // T extract_data(Buffer::iterator& iter);

    // template<typename T>
    // T extract_data(uint64_t& pos, T& data);

    // template<typename T>
    // T extract_data(Buffer::iterator& iter, T& data);

    // template<typename T>
    // void update_data(uint64_t& pos, const T& data);

    // template<typename T>
    // void update_data(Buffer::iterator& iter, const T& data);

    bool read_header(Header& header);

    bool read_page_table(PageTable& page_table);

    // bool read_metadata(Metadata& metadata, uint64_t& pos);

    // bool read_metadata_list(MetadataList& metadata_list);

    bool write_header(const Header& header);

    bool write_page_table(const PageTable& page_table);

    // bool write_metadata_list(const MetadataList& metadata_list);

    // void set_buffer_dirty(uint64_t page_index);

public:
    DataFileManager();

    DataFileManager(std::string path);

    // const std::vector<MetadataBasis>& select_metadata(uint64_t start_date, uint64_t end_date);

    bool read_page(uint64_t page_index, Buffer& buffer);

    bool write_page(uint64_t page_index, const Buffer& buffer);

    uint64_t allocate_page(PageType page_type);

    bool free_page(uint64_t page_index);

    bool load_data_file(std::string path);

    bool save_data_file();
};
