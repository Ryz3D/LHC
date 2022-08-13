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
    // var_type
    // var_name

    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
