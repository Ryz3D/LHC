#include "definition.h"

bool DefinitionToken::is_supported()
{
    return true;
}

std::vector<Token *> DefinitionToken::get_children()
{
    if (assignment == nullptr)
        return {};
    else
        return {assignment};
}

void DefinitionToken::resolve()
{
    resolved = true;
}
