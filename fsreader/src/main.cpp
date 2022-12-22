// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <cstdio>

#include "fat_image.h"
#include "structs.h"
#include "fmt_functions.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Enter image file name";
        return EXIT_FAILURE;
    }
    std::string path = argv[1];

    fat_image image{path};
    image.open();

    FAT_boot_sector_t boot_sec = image.read_boot_sector();
    std::cout << parse_boot_sec(boot_sec) << '\n';

    std::vector<FAT_dir_entry> entries = image.read_root_dir_entries();
    int root_previous_sectors = boot_sec.BPB_RsvdSecCnt + boot_sec.BPB_SectorsPerFAT * boot_sec.BPB_NumFATs;
    std::vector<std::string> file_infos = parse_root_dir_entries(entries, root_previous_sectors, boot_sec.BPB_SecPerClus);
    std::cout << get_entries_desc() << '\n';
    for (auto &file_info: file_infos) {
        std::cout << file_info << '\n';
    }

    image.close();
    return 0;
}
