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
#include "parser.h"

#define MAX_OP_LEVEL 5

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
    static err_compile compile(std::vector<Token *> tokens, std::vector<Instruction *> *buffer);
    static err_assemble assemble(std::vector<Instruction *> program, std::vector<uint8_t> *buffer);
    static std::vector<Instruction *> parse_ass(std::string str);
    static std::string to_ass(std::vector<Instruction *> program);

private:
    static uint32_t label_counter;
    static bool def_print_int;
    static bool def_mul;
    static bool def_div;
    static bool def_mod;

    static Variable *find_var(std::string var_name, std::vector<Variable *> vars);
    static err_compile evaluate_exp(ExpressionToken *exp, std::vector<Variable *> vars, std::vector<Instruction *> *buffer, uint32_t into, uint8_t op_level = 0);
    static err_compile compile_statements(std::vector<Token *> tokens, std::vector<Variable *> vars, std::vector<Instruction *> *buffer, bool main);
    static err_compile get_defs(std::vector<Token *> tokens, std::vector<DefinitionToken *> *var_defs);
};
