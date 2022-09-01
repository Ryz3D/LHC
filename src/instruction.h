#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "defines.h"

#define INS_RAM_IN 0
#define INS_RAM_P_IN 1
#define INS_A_IN 2
#define INS_B_IN 3
#define INS_RAM_OUT 4
#define INS_RAM_P_OUT 5
#define INS_ALU_ADD 6
#define INS_ALU_INV 7

class Instruction
{
public:
    Instruction(std::string label);
    Instruction(uint8_t control_word, uint8_t literal = 0, std::string comment = "");
    Instruction(uint8_t control_word, std::string label_literal, std::string comment = "");

    static Instruction *parse_ass(std::string str);
    static uint8_t parse_cw(bool out, std::string cw);
    void parse_ins_part(bool out, bool *has_literal, std::string str);
    bool literal_out();
    std::string to_ass();

    uint8_t control_word = 0;
    uint8_t literal = 0;
    std::string label_literal = "";
    std::string label = "";
    std::string comment = "";
};
