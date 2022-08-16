#include "expression.h"

bool ExpressionToken::is_supported()
{
    // check comparison type support
    return true;
}

std::vector<Token *> ExpressionToken::get_children()
{
    return content;
}

void ExpressionToken::resolve()
{
    // TODO: resolve exp_type
    resolved = true;
}
