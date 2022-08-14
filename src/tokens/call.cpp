#include "call.h"

bool CallToken::is_supported()
{
    return true;
}

std::vector<Token *> CallToken::get_children()
{
    std::vector<Token *> tokens = {};
    for (size_t i = 0; i < args.size(); i++)
        tokens.push_back(args[i]);
    return tokens;
}

void CallToken::resolve()
{
    resolved = true;
}
