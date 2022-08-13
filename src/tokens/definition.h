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
    bool is_supported() override;
    std::vector<Token *> get_children() override;
    void resolve() override;
    lhc_type var_type = lhc_type::INVALID;
    std::string var_name = "";
    AssignmentToken *assignment = nullptr;
};
