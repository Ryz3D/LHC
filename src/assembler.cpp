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

uint32_t Assembler::label_counter = 0;

err_compile Assembler::evaluate_exp(ExpressionToken *exp, std::vector<Variable *> vars, std::vector<Instruction *> *buffer, uint32_t into, std::string comment)
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
            if (dynamic_cast<VariableToken *>(exp->content[0])->negative)
                buffer->push_back(new Instruction(1 << INS_ALU_INV | 1 << INS_A_IN));
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into, comment));
            buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));
        }
        else if (dynamic_cast<LiteralInt *>(exp->content[0]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into, comment));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralInt *>(exp->content[0])->data));
        }
        else if (dynamic_cast<LiteralChar *>(exp->content[0]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into, comment));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralChar *>(exp->content[0])->data));
        }
        else if (dynamic_cast<LiteralBool *>(exp->content[0]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into, comment));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<LiteralBool *>(exp->content[0])->data ? 1 : 0));
        }
        else if (dynamic_cast<OperatorToken *>(exp->content[0]) != nullptr)
        {
            OperatorToken *op = dynamic_cast<OperatorToken *>(exp->content[0]);

            // TODO: multiple res vars or something? dynamic?
            //  -> if b is an op again, RAM_EXP_RES1 would be overwritten
            // are two per operand -> 4 enough?
            //  -> probably

            // TODO: try simplifying evaluation of operands
            //  -> if b is literal, [Literal] -> B would be possible
            //  -> if b is variable, RAM -> B would be possible
            //  -> if b is operator, we have a problem
            // if alu is required again, don't immediately load (can be ignored if resolved bottom-top)

            err_compile err = evaluate_exp(op->a, vars, buffer, RAM_EXP_RES1);
            if (err != err_compile::COMPILE_SUCCESS)
                return err;
            err = evaluate_exp(op->b, vars, buffer, RAM_EXP_RES2);
            if (err != err_compile::COMPILE_SUCCESS)
                return err;

            if (op->op == "+" || op->op == "-")
            {
                buffer->push_back(new Instruction(1 << INS_RAM_P_IN, RAM_EXP_RES1));
                buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_B_IN, 0, "RES1 -> B"));
                buffer->push_back(new Instruction(1 << INS_RAM_P_IN, RAM_EXP_RES2));
                buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_A_IN, 0, "RES2 -> A"));
                if (op->op == "-")
                    buffer->push_back(new Instruction(1 << INS_ALU_INV | 1 << INS_A_IN)); // Invert A if neccessary
                buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into, comment));     // Add into target
                buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_RAM_IN));
            }
            else if (op->op == "*" || op->op == "/")
            {
                // TODO
                std::cout << "WARNING: " << op->op << " ignored";
            }
            else if (op->op == "<" || op->op == ">" || op->op == "<=" || op->op == ">=" || op->op == "==")
            {
                std::string l_true = op->op + std::to_string(label_counter) + "true";
                std::string l_false = op->op + std::to_string(label_counter++) + "false";

                buffer->push_back(new Instruction(1 << INS_RAM_P_IN, RAM_EXP_RES1));
                buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_B_IN, 0, "RES1 -> B"));
                buffer->push_back(new Instruction(1 << INS_RAM_P_IN, RAM_EXP_RES2));
                buffer->push_back(new Instruction(1 << INS_RAM_OUT | 1 << INS_A_IN, 0, "RES2 -> A"));
                buffer->push_back(new Instruction(1 << INS_ALU_INV | 1 << INS_A_IN));
                buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_A_IN, 0, "delta (" + op->raw + ")"));
                buffer->push_back(new Instruction(1 << INS_B_IN, 0));

                if (op->op == "==")
                {
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x03));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x04));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));

                    buffer->push_back(new Instruction(1 << INS_ALU_INV | 1 << INS_A_IN, 0, "-delta"));

                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x03));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x04));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));

                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, 1));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x01));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x02));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));

                    buffer->push_back(new Instruction(l_false));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, 0));

                    buffer->push_back(new Instruction(l_true));
                }

                if (op->op == "!=")
                {
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x03));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x04));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));

                    buffer->push_back(new Instruction(1 << INS_ALU_INV | 1 << INS_A_IN, 0, "-delta"));

                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x03));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x04));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));

                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, 0));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x01));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x02));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));

                    buffer->push_back(new Instruction(l_true));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, 1));

                    buffer->push_back(new Instruction(l_false));
                }

                if (op->op == ">" || op->op == ">=")
                    buffer->push_back(new Instruction(1 << INS_ALU_INV | 1 << INS_A_IN, 0, "-delta"));
                if (op->op == "<" || op->op == "<=" || op->op == ">" || op->op == ">=")
                {
                    if (op->op == "<=" || op->op == ">=")
                    {
                        buffer->push_back(new Instruction(1 << INS_B_IN, 0xFF));
                        buffer->push_back(new Instruction(1 << INS_ALU_ADD | 1 << INS_A_IN, 0, "delta - 1"));
                    }

                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x03));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x04));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_true));

                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, 0));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x01));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x02));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, l_false));

                    buffer->push_back(new Instruction(l_true));
                    buffer->push_back(new Instruction(1 << INS_RAM_P_IN, into));
                    buffer->push_back(new Instruction(1 << INS_RAM_IN, 1));

                    buffer->push_back(new Instruction(l_false));
                }
            }
            else if (op->op == "||" || op->op == "&&")
            {
                // TODO
                std::cout << "WARNING: " << op->op << " ignored";
            }
            else
                return err_compile::COMPILE_ILLEGAL_OP;
        }
        else if (dynamic_cast<ExpressionToken *>(exp->content[0]) != nullptr)
            std::cout << "Nested expression in \"" << exp->content[0]->raw << "\", please investigate" << std::endl;
        else
            std::cout << "Can't evaluate \"" << exp->content[0]->raw << "\" (yet)" << std::endl;
    }

    // TODO: iterate with flags (i.e. B=0) to remove redundant instructions

    return err_compile::COMPILE_SUCCESS;
}

err_compile Assembler::compile(std::vector<Token *> tokens, std::vector<Instruction *> *buffer)
{
    buffer->clear();
    Assembler::label_counter = 0;

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
    //  2 expression results
    uint8_t var_offset = 10;
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
                err_compile err = Assembler::evaluate_exp(assignment->expression, vars, buffer, var->ram_location, var->var_name);
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
                err_compile err = Assembler::evaluate_exp(assignment->expression, vars, buffer, var->ram_location, var->var_name);
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
                    err_compile err = Assembler::evaluate_exp(exp, vars, buffer, 0x05, call->func_name);
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
        {
            buffer->push_back(new Instruction(dynamic_cast<LabelToken *>(main->body[i])->label));
        }
        else if (dynamic_cast<GotoToken *>(main->body[i]) != nullptr)
        {
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x01));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<GotoToken *>(main->body[i])->label));
            buffer->push_back(new Instruction(1 << INS_RAM_P_IN, 0x02));
            buffer->push_back(new Instruction(1 << INS_RAM_IN, dynamic_cast<GotoToken *>(main->body[i])->label));
        }
        else if (dynamic_cast<IfStatement *>(main->body[i]) != nullptr)
        {
            // get expression result, subtract 1
            // cond-jump after body
        }
        else if (dynamic_cast<ReturnToken *>(main->body[i]) != nullptr)
        {
            return err_compile::COMPILE_SUCCESS;
        }
        else
        {
            std::cout << "WARNING: Ignored \"" << main->body[i]->raw << "\" during compilation" << std::endl;
        }
    }

    // TODO: look at previous commits for optimization inspiration

    return err_compile::COMPILE_NO_EXIT;
}

err_assemble Assembler::assemble(std::vector<Instruction *> program, std::vector<uint8_t> *buffer)
{
    buffer->clear();

    // TODO: check if jump target has same upper IP byte as jump (before resolving labels)

    std::vector<std::string> labels = {};
    std::vector<uint16_t> label_positions = {};
    uint16_t ip = 0;
    for (size_t i = 0; i < program.size(); i++)
    {
        if (program[i]->label.size() > 0)
        {
            labels.push_back(program[i]->label);
            label_positions.push_back(ip);
        }
        else
            ip++;
    }

    for (size_t i = 0; i < program.size(); i++)
    {
        if (program[i]->label.size() > 0)
            ;
        if (program[i]->label_literal.size() > 0)
        {
            uint8_t ram_p = 0;
            for (size_t j = i; j >= 0; j--)
            {
                if (program[j]->control_word & 1 << INS_RAM_P_IN && program[j]->literal_out())
                {
                    ram_p = program[j]->literal;
                    break;
                }
            }

            uint16_t ip_literal = 0;
            bool found = false;
            for (size_t j = 0; j < labels.size(); j++)
            {
                if (program[i]->label_literal == labels[j])
                {
                    if (ram_p == 0x01 || ram_p == 0x03)
                        ip_literal = label_positions[j] >> 8;
                    else
                        ip_literal = label_positions[j] & 0xFF;
                    found = true;
                    break;
                }
            }
            if (!found)
                return err_assemble::ASSEMBLE_UNDEF_LABEL;

            buffer->push_back(program[i]->control_word);
            buffer->push_back(ip_literal);
        }
        else
        {
            buffer->push_back(program[i]->control_word);
            buffer->push_back((uint8_t)program[i]->literal);
        }
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
        else if (str[i] == '\t')
        {
            if (buffer.back() != ' ')
                buffer += ' ';
        }
        else if (str[i] != '\r')
            buffer += str[i];
    }

    return program;
}

std::string Assembler::to_ass(std::vector<Instruction *> program)
{
    bool passed_label = false;
    std::string str = "";
    for (size_t i = 0; i < program.size(); i++)
    {
        if (program[i]->label.empty())
        {
            if (passed_label)
                str += "\t";
        }
        else
            passed_label = true;

        std::string ins = program[i]->to_ass();
        if (ins.size() > 0)
            str += ins + "\n";
    }

    return str;
}
