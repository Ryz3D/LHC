#pragma once

#include <iostream>
#include <string>

#include "instruction.h"

#define RAM_SIZE 32768

class Sim
{
public:
    Sim();

    void execute(std::vector<Instruction *> program, bool debug = false);
    void execute(Instruction *ins);
    void debug_log();

    uint8_t A = 0, B = 0, RAM_P = 0;
    uint8_t ram[RAM_SIZE];
    std::string output_buffer = "";
};
