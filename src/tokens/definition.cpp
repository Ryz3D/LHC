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
    // var_type
    // var_name

    if (raw != "return 0;")
    {
        std::cout << "ERROR: Could not resolve ReturnToken \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
