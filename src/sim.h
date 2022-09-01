#pragma once

#include <iostream>
#include <string>

#include "lhc_errors.h"
#include "instruction.h"

#define RAM_SIZE 256

class Sim
{
public:
    Sim();

    err_sim execute(std::vector<Instruction *> program, size_t max_step, bool debug = false);
    err_sim execute(std::vector<uint8_t> binary, size_t max_step, bool debug = false);
    void debug_log(Instruction *ins = nullptr);

    uint8_t A = 0, B = 0, RAM_P = 0;
    uint16_t ip = 0;
    uint8_t ram[RAM_SIZE];
    std::string output_buffer = "";
};
