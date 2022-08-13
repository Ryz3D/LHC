#include "sim.h"

Sim::Sim()
{
    for (size_t i = 0; i < RAM_SIZE; i++)
        ram[i] = 0;
}

void Sim::execute(std::vector<Instruction *> program, bool debug)
{
    for (size_t step = 0; step < 100 && ram[0] < program.size(); step++)
    {
        execute(program[ram[0]++]);
        if (debug)
        {
            std::cout << "[" << (int)step << "] ";
            debug_log();
        }
    }
}

void Sim::execute(Instruction *ins)
{
    uint8_t bus = 0;

    if (ins->literal_out())
        bus = ins->literal;
    else
    {
        if (ins->control_word & (1 << INS_RAM_OUT))
            bus = ram[RAM_P];
        if (ins->control_word & (1 << INS_RAM_P_OUT))
            bus = RAM_P;
        if (ins->control_word & (1 << INS_ALU_ADD))
            bus = A + B;
        if (ins->control_word & (1 << INS_ALU_INV))
            bus = 256 - A;
    }

    if (ins->control_word & (1 << INS_RAM_IN))
    {
        if (RAM_P != 1)
            ram[RAM_P] = bus;
        else if (A & 0b10000000)
            ram[0] = bus;
    }
    if (ins->control_word & (1 << INS_RAM_P_IN))
        RAM_P = bus;
    if (ins->control_word & (1 << INS_A_IN))
        A = bus;
    if (ins->control_word & (1 << INS_B_IN))
        B = bus;
}

void Sim::debug_log()
{
    std::cout << "A: " << (int)A << " B: " << (int)B << " RP: " << (int)RAM_P << " IP: " << (int)ram[0] << std::endl
              << "RAM: ";
    for (size_t i = 0; i < 10; i++)
        std::cout << (i == 0 ? "" : "     ") << (i < 10 ? "0" : "") << (int)i << ": " << (int)ram[i] << std::endl;
}
