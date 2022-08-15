#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "expression.h"

class OperatorToken : public Token
{
public:
    bool is_supported() override;
    void resolve() override;

    std::string op = "";
    lhc_type op_type = lhc_type::INVALID;
    ExpressionToken *a = nullptr;
    ExpressionToken *b = nullptr;
};
