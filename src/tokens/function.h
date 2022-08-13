#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "definition.h"

class FunctionToken : public Token
{
public:
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;

    lhc_type func_ret_type = lhc_type::INVALID;
    std::string func_name = "";
    std::vector<DefinitionToken *> params = {};
    std::vector<Token *> body = {};
};
