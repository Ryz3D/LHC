#include "token.h"

Token::Token() {}

Token::Token(std::string raw)
{
    this->raw = raw;
}

bool Token::is_supported()
{
    return false;
}

std::vector<Token *> Token::get_children()
{
    return {};
}

void Token::resolve() {}
