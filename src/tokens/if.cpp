#include "if.h"

bool IfStatement::is_supported()
{
    return true;
}

std::vector<Token *> IfStatement::get_children()
{
    std::vector<Token *> tokens = {condition};
    for (size_t i = 0; i < body.size(); i++)
        tokens.push_back(body[i]);
    return tokens;
}

void IfStatement::resolve()
{
    resolved = true;
}
