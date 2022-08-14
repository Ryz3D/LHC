#include "fs.h"

std::string FS::read_file(std::string path)
{
    FILE *f = fopen(path.c_str(), "r");
    if (f == nullptr)
        std::cout << "ERROR: File \"" << path << "\" could not be read!";
    std::string str = "";
    while (!feof(f))
    {
        uint8_t buf[1];
        if (!fread(buf, 1, 1, f))
            break;
        str += (char)buf[0];
    }
    fclose(f);
    return str;
}

void FS::write_file(std::string path, std::string data)
{
    FILE *f = fopen(path.c_str(), "w");
    if (f == nullptr)
        std::cout << "ERROR: File \"" << path << "\" could not be written!";
    fwrite(data.data(), 1, data.size(), f);
    fclose(f);
}

void FS::write_file(std::string path, std::vector<uint8_t> data)
{
    FILE *f = fopen(path.c_str(), "w");
    if (f == nullptr)
        std::cout << "ERROR: File \"" << path << "\" could not be written!";
    fwrite(data.data(), 1, data.size(), f);
    fclose(f);
}
