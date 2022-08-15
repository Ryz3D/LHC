#include "function.h"

bool FunctionToken::is_supported()
{
    return true;
}

std::vector<Token *> FunctionToken::get_children()
{
    std::vector<Token *> tokens = {};
    for (size_t i = 0; i < params.size(); i++)
        tokens.push_back(params[i]);
    for (size_t i = 0; i < body.size(); i++)
        tokens.push_back(body[i]);
    return tokens;
}

void FunctionToken::resolve()
{
    if (func_name != "main")
    {
        std::cout << "ERROR: Only void main() function is allowed" << std::endl;
        return;
    }

    resolved = true;
}
