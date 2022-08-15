#include "goto.h"

bool GotoToken::is_supported()
{
    return true;
}

void GotoToken::resolve()
{
    if (raw.substr(0, 5) != "goto ")
    {
        std::cout << "ERROR: Could not resolve GotoToken \"" << raw << "\"" << std::endl;
        return;
    }
    label = raw.substr(5, raw.length() - 6);
    resolved = true;
}
