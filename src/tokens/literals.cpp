#include "literals.h"

bool LiteralString::is_supported()
{
    return true;
}

bool LiteralInt::is_supported()
{
    return true;
}

bool LiteralChar::is_supported()
{
    return true;
}

bool LiteralBool::is_supported()
{
    return true;
}

void LiteralString::resolve()
{
    if (raw[0] != '"' || raw.back() != '"')
    {
        std::cout << "ERROR: Could not resolve LiteralString \"" << raw << "\"" << std::endl;
        return;
    }
    data = raw.substr(1, raw.size() - 2);

    resolved = true;
}

void LiteralInt::resolve()
{
    std::cout << raw << std::endl;
    data = 0;
    for (size_t i = 0; i < raw.size(); i++)
    {
        if (raw[i] >= '0' && raw[i] <= '9')
        {
            data *= 10;
            data += raw[i] - '0';
        }
        else
        {
            std::cout << "ERROR: Could not resolve LiteralInt \"" << raw << "\"" << std::endl;
            return;
        }
    }

    resolved = true;
}

void LiteralChar::resolve()
{
    if (raw == "'\\''")
        data = '\'';
    else if (raw == "'\\'")
        data = '\\';
    else if (raw == "'\\0'")
        data = '\0';
    else if (raw == "'\\n'")
        data = '\n';
    else if (raw == "'\\r'")
        data = '\r';
    else if (raw == "'\\t'")
        data = '\t';
    else if (raw.size() == 3)
    {
        if (raw[0] != '\'' || raw[2] != '\'')
        {
            std::cout << "ERROR: Could not resolve LiteralChar \"" << raw << "\"" << std::endl;
            return;
        }
        else
            data = raw[1];
    }
    else
    {
        std::cout << "ERROR: Could not resolve LiteralChar \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}

void LiteralBool::resolve()
{
    if (raw == "true")
        data = true;
    else if (raw == "false")
        data = false;
    else
    {
        std::cout << "ERROR: Could not resolve LiteralBool \"" << raw << "\"" << std::endl;
        return;
    }

    resolved = true;
}
