#include "return.h"

bool ReturnToken::is_supported()
{
    return true;
}

void ReturnToken::resolve()
{
    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
