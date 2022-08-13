#include "label.h"

bool LabelToken::is_supported()
{
    return true;
}

void LabelToken::resolve()
{
    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
