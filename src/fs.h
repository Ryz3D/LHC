#pragma once

#include <string>
#include <vector>
#include <iostream>

class FS
{
public:
    static std::string read_file(std::string path);
    static void write_file(std::string path, std::string data);
    static void write_file(std::string path, std::vector<uint8_t> data);
};
