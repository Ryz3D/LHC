#include "assignment.h"

bool AssignmentToken::is_supported()
{
    return true;
}

std::vector<Token *> AssignmentToken::get_children()
{
    return {expression};
}

void AssignmentToken::resolve()
{
    resolved = true;
}
