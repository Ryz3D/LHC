#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "assignment.h"

class DefinitionToken : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    void resolve() override;
    lhc_type var_type = lhc_type::INVALID;
    std::string var_name = "";
};
