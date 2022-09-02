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
    for (size_t i = 0; i < data.size(); i++)
    {
        std::string sub = data.substr(i, 2);
        bool changed = true;
        if (sub == "\\\"")
            data[i] = '\"';
        else if (sub == "\\")
            data[i] = '\\';
        else if (sub == "\\0")
            data[i] = '\0';
        else if (sub == "\\n")
            data[i] = '\n';
        else if (sub == "\\r")
            data[i] = '\r';
        else if (sub == "\\t")
            data[i] = '\t';
        else
            changed = false;

        if (changed)
            data.erase(data.begin() + i + 1);
    }

    resolved = true;
}

void LiteralInt::resolve()
{
    bool negative = false;
    data = 0;
    for (size_t i = 0; i < raw.size(); i++)
    {
        if (i == 0 && raw[0] == '-')
        {
            negative = true;
        }
        else if (CHAR_IS_NUM(raw[i]))
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

    if (negative)
        data = 256 - data;
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
