#include "operator.h"

bool OperatorToken::is_supported()
{
    return true;
}

void OperatorToken::resolve()
{
    resolved = true;
}
