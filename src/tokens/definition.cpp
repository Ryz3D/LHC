#include "definition.h"

bool DefinitionToken::is_supported()
{
    return true;
}

void DefinitionToken::resolve()
{
    var_name = raw;
    resolved = true;
}
