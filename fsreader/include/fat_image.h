//
// Created by vivi on 11.12.22.
//

#ifndef FSREADER_FAT_IMAGE_H
#define FSREADER_FAT_IMAGE_H

#include "structs.h"

#include <string>
#include <fstream>
#include <vector>

class fat_image {
public:
    fat_image(std::string &image_path) : image_path(image_path) {}
    ~fat_image();

    void open();
    void close();

    FAT_boot_sector_t read_boot_sector();
    std::vector<FAT_dir_entry> read_root_dir_entries();

private:
    FAT_dir_entry get_next_entry();

    std::string image_path;
    std::ifstream image_file{};

    size_t boot_sec_offset = 0;
    size_t FAT_offset;
    size_t root_dir_offset;
    size_t data_offset;

    int max_num_root_entries;
};


#endif //FSREADER_FAT_IMAGE_H
