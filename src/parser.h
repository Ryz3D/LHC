#pragma once

#include <vector>
#include <string>

#include "lhc_errors.h"
#include "token.h"
#include "tokens/assignment.h"
#include "tokens/call.h"
#include "tokens/definition.h"
#include "tokens/function.h"
#include "tokens/if.h"
#include "tokens/expression.h"
#include "tokens/literals.h"
#include "tokens/return.h"

enum parser_state
{
    PARSE_ANY,
    PARSE_TOP_LEVEL,
    PARSE_STATEMENT,
    PARSE_EXPRESSION,
    PARSE_PARAMS
};

class Parser
{
public:
    static err_parse parse(std::string str, std::vector<Token *> *buffer, parser_state state = parser_state::PARSE_TOP_LEVEL);
    static err_resolve resolve(std::vector<Token *> tokens);
};
