#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "lhc_errors.h"
#include "token.h"
#include "tokens/assignment.h"
#include "tokens/call.h"
#include "tokens/definition.h"
#include "tokens/expression.h"
#include "tokens/for.h"
#include "tokens/function.h"
#include "tokens/goto.h"
#include "tokens/if.h"
#include "tokens/label.h"
#include "tokens/literals.h"
#include "tokens/operator.h"
#include "tokens/return.h"
#include "tokens/variable.h"
#include "tokens/while.h"

#include "instruction.h"
#include "defines.h"

class Variable
{
public:
    Variable(lhc_type var_type, std::string var_name, uint32_t ram_location);
    lhc_type var_type;
    std::string var_name;
    uint32_t ram_location;
};

class Assembler
{
public:
    static Variable *find_var(std::string var_name, std::vector<Variable *> vars);
    static err_compile evaluate_exp(ExpressionToken *exp, std::vector<Variable *> vars, std::vector<Instruction *> *buffer, uint32_t into, bool b = false);
    static err_compile compile_statements(std::vector<Token *> tokens, std::vector<Variable *> vars, std::vector<Instruction *> *buffer, bool main);

    static err_compile compile(std::vector<Token *> tokens, std::vector<Instruction *> *buffer);
    static err_assemble assemble(std::vector<Instruction *> program, std::vector<uint8_t> *buffer);
    static std::vector<Instruction *> parse_ass(std::string str);
    static std::string to_ass(std::vector<Instruction *> program);

private:
    static uint32_t label_counter;
};
