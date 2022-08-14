#include "assembler.h"

err_compile Assembler::compile(std::vector<Token *> tokens, std::vector<Instruction *> *buffer)
{
    buffer->clear();

    buffer->push_back(new Instruction(1 << INS_A_IN, 1, "i did 1+2 :)"));
    buffer->push_back(new Instruction(1 << INS_B_IN, 2));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 8));
    buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));

    return err_compile::COMPILE_SUCCESS;
}

err_assemble Assembler::assemble(std::vector<Instruction *> program, std::vector<uint8_t> *buffer)
{
    buffer->clear();

    buffer->push_back(1);
    buffer->push_back(2);
    buffer->push_back(3);

    return err_assemble::ASSEMBLE_SUCCESS;
}

std::vector<Instruction *> Assembler::parse_ass(std::string str)
{
    std::vector<Instruction *> program = {};
    std::string buffer = "";
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '\n')
        {
            program.push_back(Instruction::parse_ass(buffer));
            buffer.clear();
        }
        else if (str[i] != '\r' && str[i] != '\t')
            buffer += str[i];
    }
    return program;
}

std::string Assembler::to_ass(std::vector<Instruction *> program)
{
    std::string str = "";
    for (size_t i = 0; i < program.size(); i++)
    {
        if (program[i]->control_word == 0)
            str += "\t";
        std::string ins = program[i]->to_ass();
        if (ins.size() > 0)
            str += ins + "\n";
    }
    return str;
}
