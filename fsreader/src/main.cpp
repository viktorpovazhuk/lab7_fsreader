// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fat_image.h"

#include <iostream>
#include <fstream>
#include <cstdio>

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "Enter image file name";
        return EXIT_FAILURE;
    }
    std::string path = argv[1];
    fat_image image{path};
    std::cout << image.get_characteristics();
    std::vector<std::string> file_infos = image.get_files();
    for (auto info: file_infos) {
        std::cout << info << '\n';
    }
    return 0;
}
