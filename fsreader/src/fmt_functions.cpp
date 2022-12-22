//
// Created by vivi on 22.12.22.
//

#include <string>
#include <vector>
#include <bitset>

#include "structs.h"

#include "fmt_functions.h"

std::string parse_boot_sec(FAT_boot_sector_t &boot_sec) {
    size_t bytesPerFAT = boot_sec.BPB_SectorsPerFAT * boot_sec.BPB_bytesPerSec;
    std::string characteristics = "Sector size: " + std::to_string(boot_sec.BPB_bytesPerSec) + "\n" +
                                  "Sectors in cluster: " + std::to_string(boot_sec.BPB_SecPerClus) + "\n" +
                                  "FAT copies: " + std::to_string(boot_sec.BPB_NumFATs) + "\n" +
                                  "FAT size (sectors/bytes): " + std::to_string(boot_sec.BPB_SectorsPerFAT) + "/" + std::to_string(bytesPerFAT) + "\n" +
                                  "Max number of files in root dir: " + std::to_string(boot_sec.BPB_RootEntCnt) + "\n" +
                                  "Reserved sectors: " + std::to_string(boot_sec.BPB_RsvdSecCnt) + "\n" +
                                  "Signature correct: " + ((boot_sec.signature == 0xAA55) ? "true" : "false") + "\n";
    return characteristics;
}

std::vector<std::string> parse_root_dir_entries(std::vector<FAT_dir_entry> &root_dir_entries, int root_previous_sectors, int sectors_cluster) {
    std::vector<std::string> files_info;
    for (auto &entry: root_dir_entries) {
        char buf[300];
        std::string fmt = "%-15s | %-10u | %-10s | %-12s | %-10s | %-12s | %-15s | %-8u | %-8u";
        int file_first_clus = entry.low_bytes_first_clus;
        int first_clus = 2;
        int file_first_sector = ((file_first_clus - first_clus) * sectors_cluster) + root_previous_sectors + 1;
        snprintf(buf, sizeof(buf), fmt.c_str(), entry.parse_file_name().c_str(), entry.file_size,
                 entry.parse_create_time().c_str(), entry.parse_create_date().c_str(),
                 entry.parse_modify_time().c_str(), entry.parse_modify_date().c_str(),
                 std::bitset<6>(entry.attributes).to_string().c_str(),
                 file_first_clus, file_first_sector);
        files_info.emplace_back(buf);
    }
    return files_info;
}

std::string get_entries_desc() {
    char buf[300];
    std::string fmt = "%-15s | %-10s | %-10s | %-12s | %-10s | %-12s | %-15s | %-8s | %-8s";
    snprintf(buf, sizeof(buf), fmt.c_str(), "name", "size", "cr_time",
             "cr_date", "mf_time", "mf_date", "RO,H,S,VL,D,A", "1_clus", "1_sec");
    return std::string{buf};
}