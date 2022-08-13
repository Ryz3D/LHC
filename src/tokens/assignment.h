#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "expression.h"

class AssignmentToken : public Token
{
public:
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;
    std::string var_name;
    ExpressionToken expression;
};
