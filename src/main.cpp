#include <iostream>
#include <string>
#include <vector>

#include "fs.h"
#include "parser.h"
#include "assembler.h"
#include "sim.h"

/*
only global definitions (or save all in global list)
jumps (label, goto)
    != 0
    > 0
compare by subtract and delta < 0
instruction pointer in ram at 0x00 (NOR ram_p)
setting 0x01 redirects to 0x00 if first bit of A
*/

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: lhc [Options] [Input] [Output]" << std::endl;
        std::cout << "\tOptions:" << std::endl;
        std::cout << "\t\t-s Run simulation" << std::endl;
        std::cout << "\tInput:" << std::endl;
        std::cout << "\t\t-ic [path] C source file" << std::endl;
        std::cout << "\t\t-ia [path] Assembly file" << std::endl;
        std::cout << "\tOutput:" << std::endl;
        std::cout << "\t\t-oa [path] Assembly file" << std::endl;
        std::cout << "\t\t-ob [path] Binary file" << std::endl;
        return 0;
    }

    bool sim = false;
    std::string in_c = "";
    std::string in_ass = "";
    std::string out_ass = "";
    std::string out_bin = "";
    for (int i = 0; i < argc; i++)
    {
        std::string a = std::string(argv[i]);
        if (a == "-s")
            sim = true;
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
        err_parse err1 = Parser::parse(FS::read_file("csrc/test.c"), &tokens);
        if (err1 != err_parse::PARSE_SUCCESS)
        {
            std::cout << "ERROR: failed parse (" << err1 << ")" << std::endl;
            return err1;
        }

        err_resolve err2 = Parser::resolve(tokens);
        if (err2 != err_resolve::RESOLVE_SUCCESS)
        {
            std::cout << "ERROR: Failed resolve (" << err2 << ")" << std::endl;
            return err2;
        }

        err_compile err3 = Assembler::compile(tokens, &program);
        if (err3 != err_compile::COMPILE_SUCCESS)
        {
            std::cout << "ERROR: Failed compile (" << err3 << ")" << std::endl;
            return err3;
        }
    }
    else if (in_ass.size() > 0)
    {
        program = Assembler::parse_ass(FS::read_file(in_ass));
    }
    else
    {
        std::cout << "ERROR: No input files" << std::endl;
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

    if (sim)
    {
        Sim *cpu = new Sim();
        cpu->execute(program, true);
    }

    std::cout << "LHC Done!" << std::endl;

    return 0;
}
