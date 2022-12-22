//
// Created by vivi on 22.12.22.
//

#ifndef FSREADER_FMT_FUNCTIONS_H
#define FSREADER_FMT_FUNCTIONS_H

std::string parse_boot_sec(FAT_boot_sector_t &boot_sec);
std::vector<std::string> parse_root_dir_entries(std::vector<FAT_dir_entry> &root_dir_entries, int root_previous_sectors, int sectors_cluster);
std::string get_entries_desc();

#endif //FSREADER_FMT_FUNCTIONS_H
