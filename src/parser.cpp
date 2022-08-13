#include "parser.h"

err_parse Parser::parse(std::string str, std::vector<Token *> *buffer)
{
    buffer->clear();

    std::string token_buffer = "";
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '"')
        {
            size_t start = i++;
            while (!(str[i] == '"' && str[i] != '\\') && i < str.size())
                i++;
            if (i == str.size() - 1)
                return err_parse::PARSE_ENDING_STRING;

            LiteralString *lit = new LiteralString();
            lit->raw = str.substr(start, (++i) - start);
            buffer->push_back(lit);
        }
        if (str[i] == '\'')
        {
            size_t start = i++;
            while (!(str[i] == '\'' && str[i] != '\\') && i < str.size())
                i++;
            if (i == str.size() - 1)
                return err_parse::PARSE_ENDING_CHAR;

            LiteralChar *lit = new LiteralChar();
            lit->raw = str.substr(start, (++i) - start);
            buffer->push_back(lit);
        }
    }

    // fill body with recursive parse()!
    // split expressions into math ops until variable/literal

    return err_parse::PARSE_SUCCESS;
}

err_resolve Parser::resolve(std::vector<Token *> tokens, std::string *buffer)
{
    buffer->clear();

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (!tokens[i]->is_supported())
            return err_resolve::RESOLVE_NOT_SUPPORTED;
        if (!tokens[i]->resolved)
        {
            tokens[i]->resolve();
            if (!tokens[i]->resolved)
                return err_resolve::RESOLVE_MISSING;
        }
    }

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (dynamic_cast<AssignmentToken *>(tokens[i]) != nullptr)
            *buffer += "Assignment (" + dynamic_cast<AssignmentToken *>(tokens[i])->var_name + ")";
        else if (dynamic_cast<DefinitionToken *>(tokens[i]) != nullptr)
            *buffer += "Definition (" + dynamic_cast<DefinitionToken *>(tokens[i])->var_name + ")";
        else if (dynamic_cast<ExpressionToken *>(tokens[i]) != nullptr)
            *buffer += "Expression (" + dynamic_cast<ExpressionToken *>(tokens[i])->raw + ")";
        else if (dynamic_cast<FunctionToken *>(tokens[i]) != nullptr)
            *buffer += "Function (" + dynamic_cast<FunctionToken *>(tokens[i])->func_name + ")";
        else if (dynamic_cast<IfStatement *>(tokens[i]) != nullptr)
            *buffer += "If";
        else if (dynamic_cast<LiteralString *>(tokens[i]) != nullptr)
            *buffer += "String (" + dynamic_cast<LiteralString *>(tokens[i])->data + ")";
        else if (dynamic_cast<LiteralInt *>(tokens[i]) != nullptr)
            *buffer += "Int (" + std::to_string(dynamic_cast<LiteralInt *>(tokens[i])->data) + ")";
        else if (dynamic_cast<LiteralChar *>(tokens[i]) != nullptr)
            *buffer += "Char (" + std::to_string(dynamic_cast<LiteralChar *>(tokens[i])->data) + ")";
        else if (dynamic_cast<LiteralBool *>(tokens[i]) != nullptr)
            *buffer += "Bool (" + std::to_string(dynamic_cast<LiteralBool *>(tokens[i])->data) + ")";
        else if (dynamic_cast<ReturnToken *>(tokens[i]) != nullptr)
            *buffer += "Return";
        else
            *buffer += "???: " + tokens[i]->raw;
        *buffer += "\n";
    }

    return err_resolve::RESOLVE_SUCCESS;
}
