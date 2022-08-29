#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "fs.h"
#include "parser.h"
#include "assembler.h"
#include "sim.h"

/*
ACUTE TODO:
 - signed comparisons
 - operator (order)
 - return value from calltoken

0x00 reserved
0x01/0x02 instruction pointer
0x03/0x04 redirects to 0x01/0x02 if first bit of A (A < 0)
 ABOVE ARE APPLIED WHEN LOWER (LAST) BYTE IS SET
0x05 putchar/getchar IO
0x06/0x07 reserved

NOR first few RAM_P bits for RAM_CE
*/

void help()
{
    std::cout << "Usage: lhc [Options] [Input] [Output]" << std::endl;
    std::cout << "\tOptions:" << std::endl;
    std::cout << "\t\t-nv Non-verbose: Only output critical information" << std::endl;
    std::cout << "\t\t-s [steps || 10000] Run simulation" << std::endl;
    std::cout << "\t\t-d Simulation debug output" << std::endl;
    std::cout << "\tInput:" << std::endl;
    std::cout << "\t\t-ic [path] C source file" << std::endl;
    std::cout << "\t\t-ia [path] Assembly file" << std::endl;
    std::cout << "\tOutput:" << std::endl;
    std::cout << "\t\t-oa [path] Assembly file" << std::endl;
    std::cout << "\t\t-ob [path] Binary file" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        help();
        return 0;
    }

    bool nv = false;
    size_t sim_steps = 0;
    bool sim_debug = false;
    std::string in_c = "";
    std::string in_ass = "";
    std::string out_ass = "";
    std::string out_bin = "";
    for (int i = 0; i < argc; i++)
    {
        std::string a = std::string(argv[i]);
        if (a == "-nv")
            nv = true;
        else if (a == "-s")
        {
            sim_steps = 10000;
            if (i < argc - 1)
                if (argv[i + 1][0] >= '0' && argv[i + 1][0] <= '9')
                    sim_steps = std::stoi(argv[i + 1]);
        }
        else if (a == "-d")
            sim_debug = true;
        else if (a == "-ic" && i < argc - 1)
            in_c = argv[i + 1];
        else if (a == "-ia" && i < argc - 1)
            in_ass = argv[i + 1];
        else if (a == "-oa" && i < argc - 1)
            out_ass = argv[i + 1];
        else if (a == "-ob" && i < argc - 1)
            out_bin = argv[i + 1];
    }

    std::vector<Instruction *> program = {};

    if (in_c.size() > 0)
    {
        std::vector<Token *> tokens = {};
        err_parse err1 = Parser::parse(FS::read_file(in_c), &tokens);
        if (err1 != err_parse::PARSE_SUCCESS)
        {
            std::cout << "ERROR: Failed at parse (" << err1 << ")" << std::endl;
            return err1;
        }

        err_resolve err2 = Parser::resolve(tokens);
        if (err2 != err_resolve::RESOLVE_SUCCESS)
        {
            std::cout << "ERROR: Failed at resolve (" << err2 << ")" << std::endl;
            return err2;
        }

        err_compile err3 = Assembler::compile(tokens, &program);
        if (err3 != err_compile::COMPILE_SUCCESS)
        {
            std::cout << "ERROR: Failed at compile (" << err3 << ")" << std::endl;
            return err3;
        }
    }
    else if (in_ass.size() > 0)
        program = Assembler::parse_ass(FS::read_file(in_ass));
    else
    {
        std::cout << "ERROR: No input files" << std::endl;
        help();
        return 1;
    }

    std::vector<uint8_t> binary = {};
    err_assemble err4 = Assembler::assemble(program, &binary);
    if (err4 != err_assemble::ASSEMBLE_SUCCESS)
    {
        std::cout << "ERROR: Failed assemble (" << err4 << ")" << std::endl;
        return err4;
    }

    if (out_ass.size() > 0)
        FS::write_file(out_ass, Assembler::to_ass(program));
    if (out_bin.size() > 0)
        FS::write_file(out_bin, binary);

    if (sim_steps > 0)
    {
        Sim cpu = Sim();
        if (!nv)
            std::cout << "Simulation started" << std::endl;
        cpu.execute(program, sim_steps, sim_debug);
        if (!cpu.output_buffer.empty())
            std::cout << "Simulated Output:" << std::endl
                      << cpu.output_buffer << std::endl
                      << "--------------------------------" << std::endl;
    }

    int ram_usage = 8;
    for (size_t i = 0; i < binary.size(); i += 2)
        if (binary[i] & (1 << INS_RAM_P_IN) && binary[i + 1] > 0)
            if (binary[i + 1] >= ram_usage)
                ram_usage = binary[i + 1] + 1;
    if (!nv)
    {
        int p_rom = round(binary.size() / 327.68);
        std::cout << "ROM: " << binary.size() / 2 << "/65536 B (" << p_rom << "%)";
        if (binary.size() / 2 > 65536)
            std::cout << " (OVERFLOW!)";
        std::cout << std::endl;

        int p_ram = round(ram_usage / 2.56);
        std::cout << "RAM: " << ram_usage << "/256 B (" << p_ram << "%)" << std::endl;
        if (ram_usage > 256)
            std::cout << " (OVERFLOW!)";
        std::cout << std::endl;

        std::cout << "LHC Done!" << std::endl;
    }

    // TODO: check multiple bus-outputs before outputting binary

    return 0;
}
