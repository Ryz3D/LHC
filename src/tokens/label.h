#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"

class LabelToken : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    void resolve() override;

    std::string label;
};
