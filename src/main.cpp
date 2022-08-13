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

float?
else?
*/

int main()
{
    std::vector<Token *> tokens = {};
    err_parse err1 = Parser::parse(FS::read_file("csrc/test.c"), &tokens);
    if (err1 != err_parse::PARSE_SUCCESS)
    {
        std::cout << "ERROR: failed parse (" << err1 << ")" << std::endl;
        return err1;
    }

    std::string output = "";
    err_resolve err2 = Parser::resolve(tokens, &output);
    if (err2 != err_resolve::RESOLVE_SUCCESS)
    {
        std::cout << "ERROR: Failed resolve (" << err2 << ")" << std::endl;
        return err2;
    }

    std::vector<Instruction *> program = {};
    err_assemble err3 = Assembler::assemble(tokens, &program);
    if (err3 != err_assemble::ASSEMBLE_SUCCESS)
    {
        std::cout << "ERROR: Failed assemble (" << err3 << ")" << std::endl;
        return err3;
    }

    FS::write_file("ass/out.ass", Assembler::to_ass(program));

    Sim *cpu = new Sim();
    cpu->execute(program, false);

    return 0;
}
