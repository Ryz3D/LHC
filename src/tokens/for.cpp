#include "for.h"

bool ForLoop::is_supported()
{
    return true;
}

std::vector<Token *> ForLoop::get_children()
{
    std::vector<Token *> tokens = {condition};
    for (size_t i = 0; i < prepend.size(); i++)
        tokens.push_back(prepend[i]);
    for (size_t i = 0; i < body.size(); i++)
        tokens.push_back(body[i]);
    for (size_t i = 0; i < append.size(); i++)
        tokens.push_back(append[i]);
    return tokens;
}

void ForLoop::resolve()
{
    resolved = true;
}
