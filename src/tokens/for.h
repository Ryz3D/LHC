#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "expression.h"

class ForLoop : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;

    std::vector<Token *> prepend = {};
    ExpressionToken *condition = nullptr;
    std::vector<Token *> append = {};
    std::vector<Token *> body = {};
};
