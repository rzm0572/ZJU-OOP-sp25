#include "datafile_manager.h"
#include "page_table.h"
#include "marco.h"
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>

bool DataFileManager::read_header(Header& header) {
    uint64_t pos = HEADER_PAGE * PAGESIZE;
    for (uint64_t i = 0; i < FILE_TYPE_LENGTH; i++) {
        header.file_type[i] = buffer.extract_data<char>(pos);
    }
    header.size_page_table = buffer.extract_data<uint64_t>(pos);
    for (uint64_t i = 0; i < header.size_page_table; i++) {
        header.page_table_index.push_back(buffer.extract_data<uint64_t>(pos));
    }

    assert_true_return(header.check_file_identifier(), "Invalid data file for Personal Diary");    
    assert_true_return(header.check_valid_structure(), "Invalid data file structure");
    return true;
}

bool DataFileManager::read_page_table(PageTable& page_table) {
    page_table = PageTable();

    for (auto page_index : header.page_table_index) {
        uint64_t page_base = page_index * PAGESIZE;
        uint64_t pos = page_base;
        while (pos + sizeof(PageTableEntity) <= page_base + PAGESIZE) {
            PageTableEntity entity = buffer.extract_data<PageTableEntity>(pos);
            assert_true_return(page_table.load_page_entity(entity), "Failed to load page entity");
        }
    }

    assert_true_return(page_table.initialize_free_pages(), "Failed to initialize free page list in page table");
    return true;
}

// bool DataFileManager::read_metadata(Metadata& metadata, uint64_t& pos) {
//     metadata.diary_id = buffer.extract_data<uint64_t>(pos);
//     metadata.date = buffer.extract_data<uint64_t>(pos);
//     metadata.next_metadata_page = buffer.extract_data<uint64_t>(pos);
    
//     uint64_t title_size = buffer.extract_data<uint64_t>(pos);
//     char title_buffer[title_size];
//     for (uint64_t i = 0; i < title_size; i++) {
//         title_buffer[i] = buffer.extract_data<char>(pos);
//     }
//     metadata.title = std::string(title_buffer, title_size);

//     uint64_t page_index_list_size = buffer.extract_data<uint64_t>(pos);
//     metadata.page_index_list.clear();
//     for (uint64_t i = 0; i < page_index_list_size; i++) {
//         metadata.page_index_list.push_back(buffer.extract_data<uint64_t>(pos));
//     }
    
//     return true;
// }

// bool DataFileManager::read_metadata_list(MetadataList& metadata_list) {
//     uint64_t curr_page = header.begin_metadata;
//     uint64_t pos = curr_page * PAGESIZE;
//     uint64_t num_metadata = buffer.extract_data<uint64_t>(pos);

//     metadata_list = MetadataList();

//     if (num_metadata == 0) {
//         return true;
//     }
    
//     Metadata metadata;
//     read_metadata(metadata, pos);
//     metadata_list.add_metadata(metadata);

//     while (metadata.next_metadata_page < header.num_pages) {
//         if (metadata.next_metadata_page == curr_page) {
//             read_metadata(metadata, pos);
//             metadata_list.add_metadata(metadata);
//         } else {
//             curr_page = metadata.next_metadata_page;
//             pos = curr_page * PAGESIZE;
//             read_metadata(metadata, pos);
//             metadata_list.add_metadata(metadata);
//         }
//     }
    
//     return true;
// }

bool DataFileManager::write_header(const Header& header) {
    uint64_t pos = HEADER_PAGE * PAGESIZE;
    for (uint64_t i = 0; i < FILE_TYPE_LENGTH; i++) {
        buffer.update_data<char>(pos, header.file_type[i]);
    }
    buffer.update_data<uint64_t>(pos, header.size_page_table);
    for (auto page_index : header.page_table_index) {
        buffer.update_data<uint64_t>(pos, page_index);
    }
    
    buffer.set_buffer_dirty(HEADER_PAGE, true);
    return true;
}

bool DataFileManager::write_page_table(const PageTable& page_table) {
    if (header.page_table_index.size() == 0) {
        return false;
    }

    PageEntityList page_entity_list = page_table.get_page_entities();
    
    uint64_t index_in_page_table = 0;
    uint64_t curr_page = header.page_table_index[index_in_page_table];
    buffer.set_buffer_dirty(curr_page, true);
    uint64_t pos = curr_page * PAGESIZE;
    for (auto page_entity : page_entity_list) {
        if (pos + sizeof(PageTableEntity) > curr_page * PAGESIZE + PAGESIZE) {
            if (index_in_page_table + 1 >= header.page_table_index.size()) {
                return false;
            }
            curr_page = header.page_table_index[++index_in_page_table];
            pos = curr_page * PAGESIZE;
            buffer.set_buffer_dirty(curr_page, true);
        }
        buffer.update_data<PageTableEntity>(pos, page_entity);
    }
    return true;
}

// bool DataFileManager::write_metadata_list(const MetadataList& metadata_list) {

// }


DataFileManager::DataFileManager() {
    file_path = "";
    buffer.clear();
    header = Header(
        FILE_TYPE,
        1,
        {1}
    );
    page_table = PageTable();
    page_table.create_initial_page_table();

}

DataFileManager::DataFileManager(std::string path) {
    load_data_file(path);
}

bool DataFileManager::load_data_file(std::string path) {
    file_path = path;
    try {
        input_fs = std::ifstream(path, std::ios::binary);
        buffer.assign(std::istream_iterator<unsigned char>(input_fs), {});
        buffer.resize_buffer_dirty();
        input_fs.close();
    } catch (const std::exception e) {
        std::cout << "Error occurs when reading file " + file_path << std::endl;
        input_fs.close();
        return false;
    }


    assert_true_return(read_header(header), "Failed to read header");
    assert_true_return(read_page_table(page_table), "Failed to read page table");
    // assert_true_return(read_metadata_list(metadata_list), "Failed to read metadata list");
    
    return true;
}

bool DataFileManager::read_page(uint64_t page_index, Buffer& page_buffer) {
    assert_false_return(page_table.is_free_page(page_index), "Read free page index");

    uint64_t pos = page_index * PAGESIZE;
    page_buffer.assign(buffer.begin() + pos, buffer.begin() + pos + PAGESIZE);
    return true;
}

bool DataFileManager::write_page(uint64_t page_index, const Buffer& page_buffer) {
    assert_false_return(page_table.is_free_page(page_index), "Write to free page index");

    uint64_t pos = page_index * PAGESIZE;
    std::copy(page_buffer.begin(), page_buffer.end(), buffer.begin() + pos);
    buffer.set_buffer_dirty(page_index, true);
    return true;
}

uint64_t DataFileManager::allocate_page(PageType page_type) {
    uint64_t page_index = page_table.allocate_page(page_type);
    if (page_index == INVALID_PAGE) {
        return INVALID_PAGE;
    }
    
    uint64_t pos = page_index * PAGESIZE;
    if (buffer.size() < pos + PAGESIZE) {
        buffer.resize(pos + PAGESIZE);
        buffer.resize_buffer_dirty();
    }

    return page_index;
}

bool DataFileManager::free_page(uint64_t page_index) {
    return page_table.free_page(page_index);
}

bool DataFileManager::save_data_file() {
    write_header(header);
    write_page_table(page_table);

    try {
        output_fs = std::ofstream(file_path, std::ios::binary);
        
        for (uint64_t i = 0; i < buffer.get_buffer_dirty_size(); i++) {
            if (buffer.get_buffer_dirty(i)) {
                output_fs.seekp(i * PAGESIZE);
                output_fs.write(reinterpret_cast<const char*>(buffer.data() + i * PAGESIZE), PAGESIZE);
            }
            buffer.set_buffer_dirty(i, false);
        }
        output_fs.close();
    } catch (const std::exception e) {
        std::cout << "Error occurs when writing file " + file_path << std::endl;
        output_fs.close();
        return false;
    }
    return true;
}
