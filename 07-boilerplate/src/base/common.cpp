#include <iostream>
#include <fstream>
#include "common.hpp"

std::string read_file(const char *path) {
    std::string content;
    std::ifstream fs(path, std::ios::in);

    if(!fs.is_open()) {
        std::cerr << "Error: Could not read file " << path << ". File does not exist." << std::endl;
        return NULL;
    }

    std::string line = "";
    while(!fs.eof()) {
        std::getline(fs, line);
        content.append(line + "\n");
    }

    fs.close();
    return content;
}
