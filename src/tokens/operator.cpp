#include "operator.h"

bool OperatorToken::is_supported()
{
    return true;
}

std::vector<Token *> OperatorToken::get_children()
{
    return {a, b};
}

void OperatorToken::resolve()
{
    resolved = true;
}
