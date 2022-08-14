#pragma once

#include <vector>
#include <string>

#include "instruction.h"
#include "token.h"

class Assembler
{
public:
    static err_compile compile(std::vector<Token *> tokens, std::vector<Instruction *> *buffer);
    static err_assemble assemble(std::vector<Instruction *> program, std::vector<uint8_t> *buffer);
    static std::vector<Instruction *> parse_ass(std::string str);
    static std::string to_ass(std::vector<Instruction *> program);
};
