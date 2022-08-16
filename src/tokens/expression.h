#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"

class ExpressionToken : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;

    lhc_type exp_type = lhc_type::INVALID;
    std::vector<Token *> content = {};
};
