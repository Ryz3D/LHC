#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "expression.h"

class IfStatement : public Token
{
public:
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;

    ExpressionToken *condition = nullptr;
    std::vector<Token *> body = {};
};
