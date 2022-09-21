#include "img.h"

std::string Img::bin_to_img(std::vector<uint8_t> bin, bool odd)
{
    std::string img = "v2.0 raw\n";
    for (size_t i = 0; i < bin.size(); i++)
        if ((i % 2 == 0) ^ odd)
            img += Img::hex_str(bin.at(i)) + "\n";
    return img;
}

std::string Img::hex_str(uint8_t b)
{
    std::string hex = "";
    hex += Img::hex_chr(b / 16);
    hex += Img::hex_chr(b % 16);
    return hex;
}

char Img::hex_chr(uint8_t b)
{
    if (b >= 10)
        return 'a' + b - 10;
    else
        return '0' + b;
}
