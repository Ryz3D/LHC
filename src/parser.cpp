#include "parser.h"

err_parse Parser::parse(std::string str, std::vector<Token *> *buffer, parser_state state)
{
    // TODO: check for assignment, token_buffer.size() > 0 && str[i] == '=' && str[i+1] != '='
    // check for <= >= != etc. (only in non-expression-state)
    // convert ++ to = [] + 1
    buffer->clear();

    lhc_type parsed_type = lhc_type::INVALID;
    std::string token_buffer = "";
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '/')
        {
            if (i < str.size() - 1)
            {
                if (str[i + 1] == '/')
                    while (str[i] != '\n' && i < str.size())
                        i++;
                else if (str[i + 1] == '*')
                    while ((str[i - 1] != '*' || str[i] != '/') && i < str.size())
                        i++;
            }
        }

        if (str[i] == ';' || str[i] == ' ' || str[i] == '\n' || str[i] == ',')
        {
            bool follows_type = parsed_type != lhc_type::INVALID;

            if (token_buffer == "bool")
                parsed_type = lhc_type::BOOL;
            else if (token_buffer == "uint8_t")
                parsed_type = lhc_type::UINT8;
            else if (token_buffer == "int8_t" || token_buffer == "int")
                parsed_type = lhc_type::INT8;
            else if (token_buffer == "uint16_t")
                parsed_type = lhc_type::UINT16;
            else if (token_buffer == "int16_t")
                parsed_type = lhc_type::INT16;
            else if (token_buffer == "uint32_t")
                parsed_type = lhc_type::UINT32;
            else if (token_buffer == "int32_t")
                parsed_type = lhc_type::INT32;
            else
                parsed_type = lhc_type::INVALID;

            if (parsed_type != lhc_type::INVALID && follows_type)
                return err_parse::PARSE_UNEXPECTED_TYPE;

            if (token_buffer.size() && parsed_type == lhc_type::INVALID)
                std::cout << "(" << state << "): " << token_buffer << std::endl;

            token_buffer.clear();
        }
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
            token_buffer.push_back(str[i]);

        if (state == parser_state::PARSE_ANY || state == parser_state::PARSE_EXPRESSION)
        {
            // TODO: OPERATORS!
            // split expressions into math ops until variable/literal

            if (str[i] == '"')
            {
                size_t start = i++;
                while (!(str[i] == '"' && str[i] != '\\') && i < str.size())
                    i++;
                if (i >= str.size() - 1)
                    return err_parse::PARSE_ENDING_STRING;

                buffer->push_back(new LiteralString(str.substr(start, (++i) - start)));
            }
            else if (str[i] == '\'')
            {
                size_t start = i++;
                while (!(str[i] == '\'' && str[i] != '\\') && i < str.size())
                    i++;
                if (i >= str.size() - 1)
                    return err_parse::PARSE_ENDING_CHAR;

                buffer->push_back(new LiteralChar(str.substr(start, (++i) - start)));
            }
            else if (str[i] >= '0' && str[i] <= '9')
            {
                if (str[i == 0 ? 0 : (i - 1)] == '-')
                    token_buffer.push_back('-');
                while (str[i] >= '0' && str[i] <= '9' && i < str.size() - 1)
                    token_buffer.push_back(str[i++]);

                // TODO: OPERATORS!!!
                if (str[i] == ' ' || str[i] == ';' || str[i] == ')')
                    buffer->push_back(new LiteralInt(token_buffer));
                else
                    return err_parse::PARSE_NAN;

                token_buffer.clear();
            }
        }
        if (str[i] == '(')
        {
            if (parsed_type != lhc_type::INVALID)
            {
                if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_TOP_LEVEL)
                    return err_parse::PARSE_UNEXPECTED_FUNCTION_DEF;

                // function definition
                size_t params_start = i + 1;
                while (str[i] != ')' && i < str.size() - 1)
                    i++;
                size_t params_end = i;
                while (str[i] != '{' && i < str.size() - 1)
                    i++;
                size_t start = i + 1;
                while (str[i] != '}' && i < str.size() - 1)
                    i++;

                FunctionToken *func = new FunctionToken(str.substr(start, i - start));
                func->func_name = token_buffer;
                func->func_ret_type = parsed_type;
                err_parse err = Parser::parse(str.substr(params_start, params_end - params_start), &func->params, parser_state::PARSE_PARAMS);
                if (err != err_parse::PARSE_SUCCESS)
                    return err;
                err = Parser::parse(func->raw, &func->body, parser_state::PARSE_STATEMENT);
                if (err != err_parse::PARSE_SUCCESS)
                    return err;

                token_buffer.clear();
                buffer->push_back(func);
            }
            else
            {
                if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_STATEMENT && state != parser_state::PARSE_EXPRESSION)
                    return err_parse::PARSE_UNEXPECTED_FUNCTION_CALL;

                // function call
                size_t args_start = i + 1;
                while (str[i] != ')' && i < str.size() - 1)
                    i++;

                std::string args = str.substr(args_start, i - args_start);
                CallToken *call = new CallToken(token_buffer + "(" + args + ")");
                call->func_name = token_buffer;

                // TODO: multiple arguments, split by comma and parse each as expression
                err_parse err = Parser::parse(args, &call->args, parser_state::PARSE_EXPRESSION);
                if (err != err_parse::PARSE_SUCCESS)
                    return err;

                token_buffer.clear();
                buffer->push_back(call);
            }
        }
    }

    return err_parse::PARSE_SUCCESS;
}

err_resolve Parser::resolve(std::vector<Token *> tokens)
{
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
            std::cout << "Assignment (" + dynamic_cast<AssignmentToken *>(tokens[i])->var_name + ")";
        else if (dynamic_cast<DefinitionToken *>(tokens[i]) != nullptr)
            std::cout << "Definition (" + dynamic_cast<DefinitionToken *>(tokens[i])->var_name + ")";
        else if (dynamic_cast<ExpressionToken *>(tokens[i]) != nullptr)
            std::cout << "Expression (" + dynamic_cast<ExpressionToken *>(tokens[i])->raw + ")";
        else if (dynamic_cast<FunctionToken *>(tokens[i]) != nullptr)
            std::cout << "Function (" + dynamic_cast<FunctionToken *>(tokens[i])->func_name + ")";
        else if (dynamic_cast<IfStatement *>(tokens[i]) != nullptr)
            std::cout << "If";
        else if (dynamic_cast<LiteralString *>(tokens[i]) != nullptr)
            std::cout << "String (" + dynamic_cast<LiteralString *>(tokens[i])->data + ")";
        else if (dynamic_cast<LiteralInt *>(tokens[i]) != nullptr)
            std::cout << "Int (" + std::to_string(dynamic_cast<LiteralInt *>(tokens[i])->data) + ")";
        else if (dynamic_cast<LiteralChar *>(tokens[i]) != nullptr)
            std::cout << "Char (" + std::to_string(dynamic_cast<LiteralChar *>(tokens[i])->data) + ")";
        else if (dynamic_cast<LiteralBool *>(tokens[i]) != nullptr)
            std::cout << "Bool (" + std::to_string(dynamic_cast<LiteralBool *>(tokens[i])->data) + ")";
        else if (dynamic_cast<ReturnToken *>(tokens[i]) != nullptr)
            std::cout << "Return";
        else
            std::cout << "???: " + tokens[i]->raw;
        std::cout << "\n";
    }

    return err_resolve::RESOLVE_SUCCESS;
}
