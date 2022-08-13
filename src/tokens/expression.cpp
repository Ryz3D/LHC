#include "expression.h"

bool ExpressionToken::is_supported()
{
    // check comparison type support
    return true;
}

void ExpressionToken::resolve()
{
    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
