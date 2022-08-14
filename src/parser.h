#pragma once

#include <vector>
#include <string>

#include "lhc_errors.h"
#include "token.h"
#include "tokens/assignment.h"
#include "tokens/definition.h"
#include "tokens/function.h"
#include "tokens/if.h"
#include "tokens/expression.h"
#include "tokens/literals.h"
#include "tokens/return.h"

class Parser
{
public:
    static err_parse parse(std::string str, std::vector<Token *> *buffer);
    static err_resolve resolve(std::vector<Token *> tokens);
};
