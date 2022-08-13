#include "assignment.h"

bool AssignmentToken::is_supported()
{
    return true;
}

std::vector<Token *> AssignmentToken::get_children()
{
    return {&expression};
}

void AssignmentToken::resolve()
{
    // var_name
    // expression

    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
