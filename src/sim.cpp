#include "sim.h"

Sim::Sim()
{
    for (size_t i = 0; i < RAM_SIZE; i++)
        ram[i] = 0;
}

void Sim::execute(std::vector<Instruction *> program, size_t max_step, bool debug)
{
    for (size_t step = 0; step < max_step && ip < program.size(); step++)
    {
        Instruction *ins = program[ip];

        if (ins->label.size() == 0)
        {
            uint8_t bus = 0;
            if (ins->literal_out())
            {
                if (ins->label_literal.size() > 0)
                {
                    bool found = false;
                    for (size_t i = 0; i < program.size(); i++)
                    {
                        if (program[i]->label == ins->label_literal)
                        {
                            uint16_t to = i - 1;
                            if (RAM_P == 0x01 || RAM_P == 0x03)
                                bus = to >> 8;
                            else
                                bus = to & 0xFF;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        std::cout << "WARNING: Can't find label \"" << ins->label_literal << "\"" << std::endl;
                }
                else
                    bus = ins->literal;
            }
            else
            {
                if (ins->control_word & (1 << INS_RAM_OUT))
                {
                    if (RAM_P == 0x01)
                        bus = ip >> 8;
                    else if (RAM_P == 0x02)
                        bus = ip & 0xFF;
                    else
                        bus = ram[RAM_P];
                }
                if (ins->control_word & (1 << INS_RAM_P_OUT))
                    bus = RAM_P;
                if (ins->control_word & (1 << INS_ALU_ADD))
                    bus = A + B;
                if (ins->control_word & (1 << INS_ALU_INV))
                    bus = 256 - A;
            }

            if (ins->control_word & (1 << INS_RAM_IN))
            {
                if (RAM_P == 0x02)
                    ip = (uint16_t)ram[0x01] << 8 | bus;
                else if (RAM_P == 0x04)
                {
                    if (A & 0b10000000)
                        ip = (uint16_t)ram[0x03] << 8 | bus;
                }
                else if (RAM_P == 0x05)
                    output_buffer += (char)bus;
                else
                    ram[RAM_P] = bus;
            }
            if (ins->control_word & (1 << INS_RAM_P_IN))
                RAM_P = bus;
            if (ins->control_word & (1 << INS_A_IN))
                A = bus;
            if (ins->control_word & (1 << INS_B_IN))
                B = bus;

            if (debug)
            {
                std::cout << "[" << (int)step << "] ";
                debug_log(ins);
            }
        }
        else if (debug)
            std::cout << ";" << ins->label << std::endl
                      << std::endl;

        ip++;
    }
}

void Sim::debug_log(Instruction *ins)
{
    std::cout << ins->to_ass() << std::endl
              << "RAM: ";

    for (size_t i = 10; i <= 20; i++)
        std::cout << "[" << (int)i << "]: " << (int)ram[i] << (i == 20 ? "\n" : "  /  ");

    std::cout << "A: " << (int)A
              << " B: " << (int)B
              << " RP: " << (int)RAM_P
              << " IP: " << (int)ip;

    if (output_buffer.size() > 0)
        std::cout << " OUT: \"" << output_buffer << "\"";

    std::cout << std::endl
              << std::endl;
}
