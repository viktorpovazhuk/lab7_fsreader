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
    fat_image(std::string &image_path);
    ~fat_image();

    std::string get_characteristics();
    std::vector<std::string> get_files();

private:
    std::string get_next_file();

    std::ifstream image_file{};
    FAT_boot_sector_t boot_sector{};
    size_t bytesPerFAT;
    size_t bytesPerClus;
    size_t FAT_offset;
    size_t root_dir_offset;
    size_t data_offset;
};


#endif //FSREADER_FAT_IMAGE_H
