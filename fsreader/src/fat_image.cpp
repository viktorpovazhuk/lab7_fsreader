//
// Created by vivi on 11.12.22.
//

#include "fat_image.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

fat_image::fat_image(std::string &image_path) {
    image_file.open(image_path, std::ios::in | std::ios::binary);
    if (!image_file.is_open()) {
        throw std::runtime_error{"Error open image"};
    }

    image_file.read(reinterpret_cast<char *>(&boot_sector), sizeof(FAT_boot_sector_t));
    bytesPerFAT = boot_sector.BPB_SectorsPerFAT * boot_sector.BPB_bytesPerSec;
    bytesPerClus = boot_sector.BPB_bytesPerSec * boot_sector.BPB_SecPerClus;

    FAT_offset = boot_sector.BPB_bytesPerSec * boot_sector.BPB_RsvdSecCnt;
    root_dir_offset = FAT_offset + bytesPerFAT * boot_sector.BPB_NumFATs;
    data_offset = root_dir_offset + boot_sector.BPB_RootEntCnt * 32;
}

fat_image::~fat_image() {
    image_file.close();
}

std::string fat_image::get_characteristics() {
    std::string characteristics = "Sector size: " + std::to_string(boot_sector.BPB_bytesPerSec) + "\n" +
            "Sectors in cluster: " + std::to_string(boot_sector.BPB_SecPerClus) + "\n" +
            "FAT copies: " + std::to_string(boot_sector.BPB_NumFATs) + "\n" +
            "FAT size (sectors/bytes): " + std::to_string(boot_sector.BPB_SectorsPerFAT) + "/" + std::to_string(bytesPerFAT) + "\n" +
            "Max number of files in root dir: " + std::to_string(boot_sector.BPB_RootEntCnt) + "\n" +
            "Reserved sectors: " + std::to_string(boot_sector.BPB_RsvdSecCnt) + "\n" +
            "Signature correct: " + ((boot_sector.signature == 0xAA55) ? "true" : "false") + "\n";
    return characteristics;
}

std::string parse_date(uint16_t date) {
    int day = date % (1 << 5),
            month = (date >> 5) % (1 << 4),
            year = (date >> 9) % (1 << 7) + 1980;
    return std::to_string(year) + "/" + std::to_string(month) + "/" + std::to_string(day);
}

std::string parse_time(uint16_t time) {
    int secs = time % (1 << 5) * 2,
            mins = (time >> 5) % (1 << 6),
            hours = (time >> 11) % (1 << 5);
    return std::to_string(hours) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
}

std::string fat_image::get_next_file() {
    FAT_dir_entry entry{};
    image_file.read(reinterpret_cast<char *>(&entry), sizeof(FAT_dir_entry));

    if (entry.file_name[0] == 0x00) {
        return "";
    }

    bool is_dir = (entry.attributes & 0x10) != 0;
    char name[9], ext[4];
    for (int i = 2; i > -1; --i) {
        if (entry.file_name[8 + i] != 0x20) {
            memcpy(ext, entry.file_name + 8, i + 1);
            ext[i + 1] = '\0';
            break;
        }
        if (i == 0) {
            ext[0] = '\0';
        }
    }
    for (int i = 7; i > -1; --i) {
        if (entry.file_name[i] != 0x20) {
            memcpy(name, entry.file_name, i + 1);
            name[i + 1] = '\0';
            break;
        }
    }
    std::string file_name = (is_dir ? "/" : "") + std::string{name} + (!std::string{ext}.empty() ? "." : "") + std::string{ext};

    std::string creat_time = parse_time(entry.creat_time), creat_date = parse_date(entry.creat_date);
    std::string modif_time = parse_time(entry.modif_time), modif_date = parse_date(entry.modif_date);

    char buf[300];
    std::string fmt = "%-15s | %-10u | %-10s %-12s | %-10s %-12s";
    snprintf(buf, sizeof(buf), fmt.c_str(), file_name.c_str(), entry.file_size,
             creat_time.c_str(), creat_date.c_str(),
             modif_time.c_str(), modif_date.c_str());
    std::string file_info{buf};

    return file_info;
}

std::vector<std::string> fat_image::get_files() {
    image_file.seekg(root_dir_offset, std::ios_base::beg);
    std::vector<std::string> file_infos;
    std::string file_info;
    while (!(file_info = get_next_file()).empty()) {
        file_infos.push_back(file_info);
    }
    return file_infos;
}