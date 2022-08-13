#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../lhc_errors.h"
#include "../token.h"

class LiteralString : public Token
{
public:
    bool is_supported() override;
    void resolve() override;
    std::string data = "";
};

class LiteralInt : public Token
{
public:
    bool is_supported() override;
    void resolve() override;
    int data = 0;
};

class LiteralChar : public Token
{
public:
    bool is_supported() override;
    void resolve() override;
    char data = 0;
};

class LiteralBool : public Token
{
public:
    bool is_supported() override;
    void resolve() override;
    bool data = 0;
};
