#pragma once

#include <vector>
#include <string>

class Img
{
public:
    static std::string bin_to_img(std::vector<uint8_t> bin, bool odd);

private:
    static std::string hex_str(uint8_t b);
    static char hex_chr(uint8_t b);
};
