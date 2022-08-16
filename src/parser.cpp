#include "parser.h"

err_parse Parser::parse(std::string str_in, std::vector<Token *> *buffer, parser_state state)
{
    // check for <= >= != etc. (only in expression state)
    // convert ++ to = [] + 1
    buffer->clear();

    lhc_type parsed_type = lhc_type::INVALID;
    std::string token_buffer = "";
    std::string str = str_in + " ";
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '#')
            if (i == 0 || str[i - 1] == '\n')
                while (str[i] != '\n' && i < str.size() - 1)
                    i++;
        if (str[i] == '/')
        {
            if (i < str.size() - 1)
            {
                if (str[i + 1] == '/')
                    while (str[i] != '\n' && i < str.size() - 1)
                        i++;
                else if (str[i + 1] == '*')
                    while ((str[i - 1] != '*' || str[i] != '/') && i < str.size() - 1)
                        i++;
            }
        }

        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_')
            token_buffer.push_back(str[i]);
        else if (token_buffer.size() > 0)
        {
            lhc_type previous_type = parsed_type;
            parsed_type = Parser::parse_type(token_buffer);

            if (parsed_type != lhc_type::INVALID && previous_type != lhc_type::INVALID)
                return err_parse::PARSE_UNEXPECTED_TYPE;

            if (parsed_type == lhc_type::INVALID)
            {
                if (!Parser::parse_keyword(token_buffer, str, buffer, &i))
                {
                    if (state == parser_state::PARSE_EXPRESSION)
                        buffer->push_back(new VariableToken(token_buffer));
                    else
                    {
                        DefinitionToken *def = new DefinitionToken();
                        if (previous_type != lhc_type::INVALID)
                        {
                            def->var_name = token_buffer;
                            def->var_type = previous_type;
                            buffer->push_back(def);
                        }
                    }
                    while ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n') && i < str.size() - 1)
                        i++;
                    if (str[i] == ',')
                    {
                        if (state != parser_state::PARSE_PARAMS)
                            return err_parse::PARSE_MULTI_DEF;
                        else if (i < str.size() - 1)
                            i++;
                        token_buffer.clear();
                    }
                    else if (str[i] == '(')
                    {
                        if (previous_type == lhc_type::INVALID) // FUNCTION CALL
                        {
                            if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_STATEMENT && state != parser_state::PARSE_EXPRESSION)
                                return err_parse::PARSE_UNEXPECTED_FUNCTION_CALL;

                            size_t args_start = i + 1;
                            while (str[i] != ')' && i < str.size() - 1)
                                i++;

                            std::string args = str.substr(args_start, i - args_start);
                            CallToken *call = new CallToken(token_buffer + "(" + args + ")");
                            call->func_name = token_buffer;
                            ExpressionToken *exp = new ExpressionToken(args);
                            // TODO: multiple arguments, split by comma and parse each as expression
                            err_parse err = Parser::parse(args, &exp->content, parser_state::PARSE_EXPRESSION);
                            if (err != err_parse::PARSE_SUCCESS)
                                return err;
                            call->args.push_back(exp);

                            buffer->push_back(call);
                        }
                        else // FUNCTION DEFINITION
                        {
                            if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_TOP_LEVEL)
                                return err_parse::PARSE_UNEXPECTED_FUNCTION_DEF;

                            size_t params_start = i + 1;
                            while (str[i] != ')' && i < str.size() - 1)
                                i++;
                            size_t params_end = i;
                            while (str[i] != '{' && i < str.size() - 1)
                                i++;
                            size_t start = i + 1;
                            while (str[i] != '}' && i < str.size() - 1)
                                i++;

                            // remove DefinitionToken
                            buffer->pop_back();
                            FunctionToken *func = new FunctionToken(str.substr(start, i - start));
                            func->func_name = token_buffer;
                            func->func_ret_type = parsed_type;
                            err_parse err = Parser::parse(str.substr(params_start, params_end - params_start), &func->params, parser_state::PARSE_PARAMS);
                            if (err != err_parse::PARSE_SUCCESS)
                                return err;
                            err = Parser::parse(func->raw, &func->body, parser_state::PARSE_STATEMENT);
                            if (err != err_parse::PARSE_SUCCESS)
                                return err;

                            buffer->push_back(func);
                        }
                        token_buffer.clear();
                    }
                    else if (str[i] == ':')
                    {
                        LabelToken *label = new LabelToken(token_buffer + ":");
                        label->label = token_buffer;
                        buffer->push_back(label);
                        token_buffer.clear();
                    }
                    else if (str[i] != ';')
                    {
                        if (i >= str.size() - 1)
                        {
                            if (state == parser_state::PARSE_PARAMS || state == parser_state::PARSE_EXPRESSION)
                                break;
                            else
                                return err_parse::PARSE_ENDING_ASSIGNMENT;
                        }

                        bool inc = false, dec = false;
                        if (str[i] == '+' && str[i + 1] == '+')
                            inc = true;
                        if (str[i] == '-' && str[i + 1] == '-')
                            dec = true;
                        if (inc || dec)
                        {
                            AssignmentToken *assignment = new AssignmentToken();
                            assignment->var_name = token_buffer;
                            assignment->expression = new ExpressionToken(token_buffer + (inc ? "+1" : "-1"));
                            OperatorToken *op = new OperatorToken();
                            op->a = new ExpressionToken(token_buffer);
                            op->a->content.push_back(new VariableToken(token_buffer));
                            op->op = inc ? "+" : "-";
                            op->b = new ExpressionToken("1");
                            op->b->content.push_back(new LiteralInt("1"));
                            assignment->expression->content.push_back(op);
                            buffer->push_back(assignment);

                            while (str[i] != ';' && i < str.size() - 1)
                                i++;
                            if (i < str.size() - 1)
                                i++;
                        }

                        if (str[i] == '=' && str[i + 1] != '=')
                        {
                            uint8_t special = 0;
                            switch (str[i - 1])
                            {
                            case '+':
                                special = 1;
                                break;
                            case '-':
                                special = 2;
                                break;
                            case '*':
                                special = 3;
                                break;
                            case '/':
                                special = 4;
                                break;
                            default:
                                break;
                            }

                            if (i < str.size() - 1)
                                i++;
                            while ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n') && i < str.size() - 1)
                                i++;
                            size_t exp_start = i;
                            while (str[i] != ';' && i < str.size() - 1)
                                i++;
                            std::string exp_str = str.substr(exp_start, i - exp_start);
                            if (i < str.size() - 1)
                                i++;

                            AssignmentToken *assignment = new AssignmentToken();
                            assignment->var_name = token_buffer;
                            assignment->expression = new ExpressionToken(exp_str);
                            err_parse err = Parser::parse(exp_str, &assignment->expression->content, parser_state::PARSE_EXPRESSION);
                            if (err != err_parse::PARSE_SUCCESS)
                                return err;
                            buffer->push_back(assignment);
                        }
                    }
                    else
                        std::cout << "(" << state << "): " << token_buffer << " " << str[i] << " at " << (int)i << std::endl;
                }
            }

            token_buffer.clear();
        }

        if (state == parser_state::PARSE_ANY || state == parser_state::PARSE_EXPRESSION)
        {
            // TODO: split expressions into Operators until Variable / Literal

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
                if (i < str.size() - 1)
                    i++;
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
    }

    return err_parse::PARSE_SUCCESS;
}

err_resolve Parser::resolve(std::vector<Token *> tokens, bool debug)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (!tokens[i]->resolved)
        {
            tokens[i]->resolve();
            if (!tokens[i]->resolved)
                return err_resolve::RESOLVE_MISSING;
            if (!tokens[i]->is_supported())
                return err_resolve::RESOLVE_NOT_SUPPORTED;

            std::vector<Token *> children = tokens[i]->get_children();
            if (children.size() > 0)
            {
                err_resolve err = Parser::resolve(children);
                if (err != err_resolve::RESOLVE_SUCCESS)
                    return err;
            }
        }
    }

    if (debug)
    {
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (dynamic_cast<AssignmentToken *>(tokens[i]) != nullptr)
                std::cout << "Assignment (" << dynamic_cast<AssignmentToken *>(tokens[i])->var_name << "=" << dynamic_cast<AssignmentToken *>(tokens[i])->expression->raw << ")";
            else if (dynamic_cast<CallToken *>(tokens[i]) != nullptr)
            {
                std::cout << "Call (" + dynamic_cast<CallToken *>(tokens[i])->func_name + " with";
                if (dynamic_cast<CallToken *>(tokens[i])->args.size() > 0)
                    std::cout << " " << dynamic_cast<CallToken *>(tokens[i])->args[0]->raw << ")";
                else
                    std::cout << "out)";
            }
            else if (dynamic_cast<DefinitionToken *>(tokens[i]) != nullptr)
                std::cout << "Definition (" + dynamic_cast<DefinitionToken *>(tokens[i])->var_name + ")";
            else if (dynamic_cast<ExpressionToken *>(tokens[i]) != nullptr)
                std::cout << "Expression (" + dynamic_cast<ExpressionToken *>(tokens[i])->raw + ") ";
            else if (dynamic_cast<FunctionToken *>(tokens[i]) != nullptr)
            {
                std::cout << "Function (" + dynamic_cast<FunctionToken *>(tokens[i])->func_name + ")" << std::endl;
                std::cout << "BODY" << std::endl;
                err_resolve err = Parser::resolve(dynamic_cast<FunctionToken *>(tokens[i])->body, true);
                if (err != err_resolve::RESOLVE_SUCCESS)
                    return err;
                std::cout << "/BODY" << std::endl;
            }
            else if (dynamic_cast<GotoToken *>(tokens[i]) != nullptr)
                std::cout << "Goto (" + dynamic_cast<GotoToken *>(tokens[i])->label + ")";
            else if (dynamic_cast<IfStatement *>(tokens[i]) != nullptr)
                std::cout << "If";
            else if (dynamic_cast<LabelToken *>(tokens[i]) != nullptr)
                std::cout << "Label (" + dynamic_cast<LabelToken *>(tokens[i])->label + ")";
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
            std::cout << std::endl;
        }
    }

    return err_resolve::RESOLVE_SUCCESS;
}

lhc_type Parser::parse_type(std::string str)
{
    if (str == "bool")
        return lhc_type::BOOL;
    else if (str == "uint8_t")
        return lhc_type::UINT8;
    else if (str == "int8_t" || str == "int")
        return lhc_type::INT8;
    else if (str == "uint16_t")
        return lhc_type::UINT16;
    else if (str == "int16_t")
        return lhc_type::INT16;
    else if (str == "uint32_t")
        return lhc_type::UINT32;
    else if (str == "int32_t")
        return lhc_type::INT32;
    else
        return lhc_type::INVALID;
}

bool Parser::parse_keyword(std::string kw, std::string str, std::vector<Token *> *buffer, size_t *i)
{
    if (kw == "return")
    {
        size_t start = *i;
        while (str[*i] != ';' && *i < str.size() - 1)
            (*i)++;
        buffer->push_back(new ReturnToken("return" + str.substr(start, 1 + (*i) - start)));
        if (*i < str.size() - 1)
            (*i)++;
        return true;
    }
    else if (kw == "goto")
    {
        size_t start = *i;
        while (str[*i] != ';' && *i < str.size() - 1)
            (*i)++;
        buffer->push_back(new GotoToken("goto" + str.substr(start, 1 + (*i) - start)));
        if (*i < str.size() - 1)
            (*i)++;
        return true;
    }
    else
        return false;
}
