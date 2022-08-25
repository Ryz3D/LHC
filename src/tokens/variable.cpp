#include "variable.h"

bool VariableToken::is_supported()
{
    return true;
}

void VariableToken::resolve()
{
    if (raw[0] == '-')
    {
        raw = raw.substr(1);
        negative = true;
    }
    resolved = true;
}
