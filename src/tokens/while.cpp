#include "while.h"

bool WhileLoop::is_supported()
{
    return true;
}

std::vector<Token *> WhileLoop::get_children()
{
    std::vector<Token *> tokens = {condition};
    for (size_t i = 0; i < body.size(); i++)
        tokens.push_back(body[i]);
    return tokens;
}

void WhileLoop::resolve()
{
    resolved = true;
}
