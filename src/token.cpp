#include "token.h"

bool Token::is_supported()
{
    return false;
}

std::vector<Token *> Token::get_children()
{
    return {};
}

void Token::resolve() {}
