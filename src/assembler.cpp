#include "assembler.h"

Variable::Variable(lhc_type var_type, std::string var_name, uint32_t ram_location)
{
    this->var_type = var_type;
    this->var_name = var_name;
    this->ram_location = ram_location;
}

Variable *Assembler::find_var(std::string var_name, std::vector<Variable *> vars)
{
    for (size_t i = 0; i < vars.size(); i++)
        if (var_name == vars[i]->var_name)
            return vars[i];
    return nullptr;
}

// TODO: use two reserved addresses for operators?
err_compile Assembler::evaluate_exp(ExpressionToken *exp, std::vector<Variable *> vars, std::vector<Instruction *> *buffer, uint32_t into)
{
    if (exp->content.size() > 0)
    {
        if (dynamic_cast<VariableToken *>(exp->content[0]) != nullptr)
        {
            Variable *var = Assembler::find_var(dynamic_cast<VariableToken *>(exp->content[0])->raw, vars);
            if (var == nullptr)
                return err_compile::COMPILE_UNDEF_VAR;

            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, var->ram_location));
            buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_A_IN));
            buffer->push_back(new Instruction(1 << INS_B_IN, 0));
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
            buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));
        }
        else if (dynamic_cast<LiteralInt *>(exp->content[0]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralInt *>(exp->content[0])->data));
        }
        else if (dynamic_cast<LiteralChar *>(exp->content[0]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralChar *>(exp->content[0])->data));
        }
        else if (dynamic_cast<LiteralBool *>(exp->content[0]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralBool *>(exp->content[0])->data ? 1 : 0));
        }
        else if (dynamic_cast<OperatorToken *>(exp->content[0]) != nullptr)
        {
            // TODO: add/sub, mul/div later
            //  buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
            //  buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralBool *>(exp->content[0])->data ? 1 : 0));
        }
        else
            std::cout << "i cant eval " << exp->content[0]->raw << " yet" << std::endl;
    }

    return err_compile::COMPILE_SUCCESS;
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

    // var offset:
    //  8 default offset
    //  1 expression result
    uint8_t var_offset = 9;
    std::vector<Variable *> vars = {};
    for (size_t i = 0; i < var_defs.size(); i++)
        vars.push_back(new Variable(var_defs[i]->var_type, var_defs[i]->var_name, i + var_offset));

    // VARIABLES ALLOCATED

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (dynamic_cast<AssignmentToken *>(tokens[i]) != nullptr)
        {
            AssignmentToken *assignment = dynamic_cast<AssignmentToken *>(tokens[i]);
            Variable *var = Assembler::find_var(assignment->var_name, vars);
            if (var == nullptr)
                return err_compile::COMPILE_UNDEF_VAR;
            if (assignment->expression != nullptr)
            {
                err_compile err = Assembler::evaluate_exp(assignment->expression, vars, buffer, var->ram_location);
                if (err != err_compile::COMPILE_SUCCESS)
                    return err;
            }
            else
                return err_compile::COMPILE_MISSING_EXP;
        }
    }

    for (size_t i = 0; i < main->body.size(); i++)
    {
        if (dynamic_cast<DefinitionToken *>(main->body[i]) != nullptr)
            ;
        else if (dynamic_cast<AssignmentToken *>(main->body[i]) != nullptr)
        {
            AssignmentToken *assignment = dynamic_cast<AssignmentToken *>(main->body[i]);
            Variable *var = Assembler::find_var(assignment->var_name, vars);
            if (var == nullptr)
                return err_compile::COMPILE_UNDEF_VAR;

            if (assignment->expression != nullptr)
            {
                err_compile err = Assembler::evaluate_exp(assignment->expression, vars, buffer, var->ram_location);
                if (err != err_compile::COMPILE_SUCCESS)
                    return err;
            }
            else
                return err_compile::COMPILE_MISSING_EXP;
        }
        else if (dynamic_cast<CallToken *>(main->body[i]) != nullptr)
        {
            CallToken *call = dynamic_cast<CallToken *>(main->body[i]);
            ExpressionToken *exp = nullptr;
            if (call->args.size() > 0)
                exp = dynamic_cast<ExpressionToken *>(call->args[0]);

            if (call->func_name == "putchar")
            {
                if (exp != nullptr)
                {
                    err_compile err = Assembler::evaluate_exp(exp, vars, buffer, 0x05);
                    if (err != err_compile::COMPILE_SUCCESS)
                        return err;
                }
                else
                    return err_compile::COMPILE_MISSING_ARG;
            }
            else if (call->func_name == "getchar")
            {
                // TODO
            }
            else
                return err_compile::COMPILE_UNDEF_FUNC;
        }
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
