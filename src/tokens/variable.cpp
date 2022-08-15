#include "variable.h"

bool VariableToken::is_supported()
{
    return true;
}

void VariableToken::resolve()
{
    resolved = true;
}
