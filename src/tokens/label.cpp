#include "label.h"

bool LabelToken::is_supported()
{
    return true;
}

void LabelToken::resolve()
{
    resolved = true;
}
