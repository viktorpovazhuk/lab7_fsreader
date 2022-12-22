//
// Created by vivi on 11.12.22.
//

#include "fat_image.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

fat_image::~fat_image() {
    if (image_file.is_open()) {
        close();
    }
}

FAT_dir_entry fat_image::get_next_entry() {
    FAT_dir_entry entry{};
    image_file.read(reinterpret_cast<char *>(&entry), sizeof(FAT_dir_entry));

    return entry;
}

std::vector<FAT_dir_entry> fat_image::read_root_dir_entries() {
    image_file.seekg(root_dir_offset, std::ios_base::beg);

    std::vector<FAT_dir_entry> entries;
    FAT_dir_entry entry{};
    int entry_num = 0;
    while (!(entry = get_next_entry()).is_available() && entry_num < max_num_root_entries) {
        if (entry.is_lfn_part() || entry.is_deleted()) continue;
        entries.push_back(entry);
        entry_num++;
    }

    return entries;
}

void fat_image::open() {
    image_file.open(image_path, std::ios::in | std::ios::binary);
    if (!image_file.is_open()) {
        throw std::runtime_error{"Error open image"};
    }
}

void fat_image::close() {
    image_file.close();
}

FAT_boot_sector_t fat_image::read_boot_sector() {
    image_file.seekg(boot_sec_offset, std::ios_base::beg);

    FAT_boot_sector_t boot_sec{};
    image_file.read(reinterpret_cast<char *>(&boot_sec), sizeof(FAT_boot_sector_t));

    int bytesPerFAT = boot_sec.BPB_SectorsPerFAT * boot_sec.BPB_bytesPerSec;
    int bytesPerClus = boot_sec.BPB_bytesPerSec * boot_sec.BPB_SecPerClus;

    FAT_offset = boot_sec.BPB_bytesPerSec * boot_sec.BPB_RsvdSecCnt;
    root_dir_offset = FAT_offset + bytesPerFAT * boot_sec.BPB_NumFATs;
    data_offset = root_dir_offset + boot_sec.BPB_RootEntCnt * 32;

    max_num_root_entries = boot_sec.BPB_RootEntCnt;

    return boot_sec;
}
