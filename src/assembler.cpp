#include "assembler.h"

Variable::Variable(lhc_type var_type, std::string var_name, uint32_t ram_location)
{
    this->var_type = var_type;
    this->var_name = var_name;
    this->ram_location = ram_location;
}

err_compile Assembler::compile(std::vector<Token *> tokens, std::vector<Instruction *> *buffer)
{
    buffer->clear();

    FunctionToken *main = nullptr;
    std::vector<DefinitionToken *> var_defs = {};
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (dynamic_cast<FunctionToken *>(tokens[i]) != nullptr)
        {
            if (dynamic_cast<FunctionToken *>(tokens[i])->func_name == "main")
                main = dynamic_cast<FunctionToken *>(tokens[i]);
        }
        else if (dynamic_cast<DefinitionToken *>(tokens[i]) != nullptr)
            var_defs.push_back(dynamic_cast<DefinitionToken *>(tokens[i]));
    }
    if (main == nullptr)
        return err_compile::COMPILE_NO_MAIN;

    for (size_t i = 0; i < main->body.size(); i++)
        if (dynamic_cast<DefinitionToken *>(main->body[i]) != nullptr)
            var_defs.push_back(dynamic_cast<DefinitionToken *>(main->body[i]));

    std::vector<Variable *> vars = {};
    for (size_t i = 0; i < var_defs.size(); i++)
        vars.push_back(new Variable(var_defs[i]->var_type, var_defs[i]->var_name, 4 + i));

    // VARIABLES ALLOCATED

    for (size_t i = 0; i < main->body.size(); i++)
    {
        if (dynamic_cast<DefinitionToken *>(main->body[i]) != nullptr)
            ;
        else if (dynamic_cast<AssignmentToken *>(main->body[i]) != nullptr)
        {
            AssignmentToken *assignment = dynamic_cast<AssignmentToken *>(main->body[i]);
            size_t index = -1;
            for (size_t i = 0; i < vars.size(); i++)
                if (assignment->var_name == vars[i]->var_name)
                    index = i;

            // TODO: figure out expression first, maybe cache it in pre-allocated location
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, vars[index]->ram_location));
        }
        else if (dynamic_cast<CallToken *>(main->body[i]) != nullptr)
            ; // TODO
        else if (dynamic_cast<LabelToken *>(main->body[i]) != nullptr)
            ; // TODO
        else if (dynamic_cast<GotoToken *>(main->body[i]) != nullptr)
            ; // TODO
        else if (dynamic_cast<IfStatement *>(main->body[i]) != nullptr)
            ; // TODO
        else if (dynamic_cast<ReturnToken *>(main->body[i]) != nullptr)
            return err_compile::COMPILE_SUCCESS;
        else
            std::cout << "ignored \"" << main->body[i]->raw << "\"" << std::endl;
    }

    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 6));
    buffer->push_back(new Instruction(1 << INS_A_IN | 1 << INS_RAM_IN, 1, "i did 1+2 :)"));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 7));
    buffer->push_back(new Instruction(1 << INS_B_IN | 1 << INS_RAM_IN, 2));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 8));
    buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));

    buffer->push_back(new Instruction(1 << INS_A_IN, '0'));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 6, "load initial A"));
    buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_B_IN));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 2));
    buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));

    buffer->push_back(new Instruction(1 << INS_RAM_IN, '+'));

    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 7, "load initial B"));
    buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_B_IN));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 2));
    buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));

    buffer->push_back(new Instruction(1 << INS_RAM_IN, '='));

    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 8, "load result"));
    buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_B_IN));
    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 2));
    buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));

    return err_compile::COMPILE_NO_EXIT;
}

err_assemble Assembler::assemble(std::vector<Instruction *> program, std::vector<uint8_t> *buffer)
{
    buffer->clear();

    // TODO: assemble labels and jumps
    for (size_t i = 0; i < program.size(); i++)
    {
        buffer->push_back(program[i]->control_word);
        buffer->push_back((uint8_t)program[i]->literal);
    }

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
            Instruction *ins = Instruction::parse_ass(buffer);
            if (ins != nullptr)
                program.push_back(ins);
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
