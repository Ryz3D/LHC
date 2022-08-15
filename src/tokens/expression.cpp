#include "expression.h"

bool ExpressionToken::is_supported()
{
    // check comparison type support
    return true;
}

void ExpressionToken::resolve()
{
    // TODO: resolve exp_type
    resolved = true;
}
