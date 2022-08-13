#include "goto.h"

bool GotoToken::is_supported()
{
    return true;
}

void GotoToken::resolve()
{
    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
