#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "definition.h"

class CallToken : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;

    std::string func_name = "";
    std::vector<Token *> args = {};
};
