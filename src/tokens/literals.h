#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"
#include "../defines.h"

class LiteralString : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    void resolve() override;
    std::string data = "";
};

class LiteralInt : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    void resolve() override;
    int data = 0;
};

class LiteralChar : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    void resolve() override;
    char data = 0;
};

class LiteralBool : public Token
{
public:
    using Token::Token;
    bool is_supported() override;
    void resolve() override;
    bool data = 0;
};
