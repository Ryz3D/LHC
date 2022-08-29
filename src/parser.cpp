#include "parser.h"

err_parse Parser::parse(std::string str_in, std::vector<Token *> *buffer, parser_state state)
{
    buffer->clear();

    std::vector<std::string> exp_operators = {};
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

        if (CHAR_IS_NAME(str[i]))
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
                    DefinitionToken *def = new DefinitionToken(token_buffer);
                    if (previous_type != lhc_type::INVALID)
                    {
                        def->var_type = previous_type;
                        buffer->push_back(def);
                    }
                    while (CHAR_IS_EMPTY(str[i]) && i < str.size() - 1)
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
                            size_t args_start = ++i;
                            Parser::find_end(str, &i, ')');
                            std::string args = str.substr(args_start, i - args_start - 1);

                            if (token_buffer == "if")
                            {
                                if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_STATEMENT)
                                    return err_parse::PARSE_UNEXPECTED_STATEMENT;

                                while (str[i] != '{' && i < str.size() - 1)
                                    i++;
                                size_t start = ++i;
                                Parser::find_end(str, &i);
                                std::string body = str.substr(start, i - start - 1);

                                IfStatement *statement = new IfStatement("if (" + args + ") {" + body + "}");
                                statement->condition = new ExpressionToken(args);
                                err_parse err = Parser::parse(args, &statement->condition->content, parser_state::PARSE_EXPRESSION);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                err = Parser::parse(body, &statement->body, parser_state::PARSE_STATEMENT);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                buffer->push_back(statement);
                            }
                            else if (token_buffer == "while")
                            {
                                if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_STATEMENT)
                                    return err_parse::PARSE_UNEXPECTED_STATEMENT;

                                while (str[i] != '{' && i < str.size() - 1)
                                    i++;
                                size_t start = ++i;
                                Parser::find_end(str, &i);
                                std::string body = str.substr(start, i - start - 1);

                                WhileLoop *loop = new WhileLoop("while (" + args + ") {" + body + "}");
                                loop->condition = new ExpressionToken(args);
                                err_parse err = Parser::parse(args, &loop->condition->content, parser_state::PARSE_EXPRESSION);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                err = Parser::parse(body, &loop->body, parser_state::PARSE_STATEMENT);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                buffer->push_back(loop);
                            }
                            else if (token_buffer == "for")
                            {
                                if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_STATEMENT)
                                    return err_parse::PARSE_UNEXPECTED_STATEMENT;

                                while (str[i] != '{' && i < str.size() - 1)
                                    i++;
                                size_t start = ++i;
                                Parser::find_end(str, &i);
                                std::string body = str.substr(start, i - start - 1);
                                std::string prepend = "", condition = "", append = "";
                                for (size_t j = 0; j < args.size(); j++)
                                {
                                    if (args[j] == ';')
                                    {
                                        if (prepend.size() == 0)
                                        {
                                            prepend = append;
                                            append.clear();
                                        }
                                        else if (condition.size() == 0)
                                        {
                                            condition = append;
                                            append.clear();
                                        }
                                        else
                                            append.clear();
                                    }
                                    else
                                        append.push_back(args[j]);
                                }

                                ForLoop *loop = new ForLoop("for (" + args + ") {" + body + "}");
                                loop->condition = new ExpressionToken(condition);
                                err_parse err = Parser::parse(prepend, &loop->prepend, parser_state::PARSE_STATEMENT);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                err = Parser::parse(condition, &loop->condition->content, parser_state::PARSE_EXPRESSION);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                err = Parser::parse(append, &loop->append, parser_state::PARSE_STATEMENT);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                err = Parser::parse(body, &loop->body, parser_state::PARSE_STATEMENT);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;

                                buffer->push_back(loop);
                            }
                            else
                            {
                                if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_STATEMENT && state != parser_state::PARSE_EXPRESSION)
                                    return err_parse::PARSE_UNEXPECTED_FUNCTION_CALL;

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
                        }
                        else // FUNCTION DEFINITION
                        {
                            if (state != parser_state::PARSE_ANY && state != parser_state::PARSE_TOP_LEVEL)
                                return err_parse::PARSE_UNEXPECTED_FUNCTION_DEF;

                            size_t params_start = ++i;
                            Parser::find_end(str, &i, ')');
                            size_t params_end = i;
                            while (str[i] != '{' && i < str.size() - 1)
                                i++;
                            size_t start = ++i;
                            Parser::find_end(str, &i);

                            // remove DefinitionToken
                            buffer->pop_back();
                            FunctionToken *func = new FunctionToken(str.substr(start, i - start - 1));
                            func->func_name = token_buffer;
                            func->func_ret_type = previous_type;
                            err_parse err = Parser::parse(str.substr(params_start, params_end - params_start - 1), &func->params, parser_state::PARSE_PARAMS);
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

                        if (state == parser_state::PARSE_STATEMENT || state == parser_state::PARSE_ANY)
                        {
                            if (CHAR_IS_ASSIGN_OP(str[i]) && str[i + 1] == '=')
                                i++;
                            else
                            {
                                bool inc = false, dec = false;
                                if (str[i] == '+' && str[i + 1] == '+')
                                    inc = true;
                                if (str[i] == '-' && str[i + 1] == '-')
                                    dec = true;
                                if (inc || dec)
                                {
                                    AssignmentToken *assignment = new AssignmentToken();
                                    assignment->var_name = token_buffer;
                                    assignment->expression = new ExpressionToken(token_buffer + (inc ? " + 1" : " - 1"));
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
                            }

                            if (str[i] == '=' && str[i + 1] != '=')
                            {
                                char op = str[i - 1];

                                if (i < str.size() - 1)
                                    i++;
                                while (CHAR_IS_EMPTY(str[i]) && i < str.size() - 1)
                                    i++;
                                size_t exp_start = i;
                                while (str[i] != ';' && i < str.size() - 1)
                                    i++;
                                std::string exp_str = str.substr(exp_start, i - exp_start);
                                if (i < str.size() - 1)
                                    i++;

                                if (CHAR_IS_OP(op))
                                    exp_str = token_buffer + op + exp_str;

                                AssignmentToken *assignment = new AssignmentToken();
                                assignment->var_name = token_buffer;
                                assignment->expression = new ExpressionToken(exp_str);
                                err_parse err = Parser::parse(exp_str, &assignment->expression->content, parser_state::PARSE_EXPRESSION);
                                if (err != err_parse::PARSE_SUCCESS)
                                    return err;
                                buffer->push_back(assignment);
                            }
                        }
                    }
                }
            }

            token_buffer.clear();
        }

        // TODO: parse expressions in parantheses recursively

        if (state == parser_state::PARSE_ANY || state == parser_state::PARSE_EXPRESSION)
        {
            err_parse err = Parser::parse_expression_part(str, buffer, &exp_operators, &i, &token_buffer);
            if (err != err_parse::PARSE_SUCCESS)
                return err;
        }
    }

    if (state == parser_state::PARSE_ANY || state == parser_state::PARSE_EXPRESSION)
    {
        if (buffer->size() <= 1)
            return err_parse::PARSE_SUCCESS;
        else
        {
            if (buffer->size() < exp_operators.size() + 1)
                return err_parse::PARSE_UNEXPECTED_OP;
            if (buffer->size() > exp_operators.size() + 1)
                return err_parse::PARSE_UNEXPECTED_TOKEN;

            for (int8_t l = 0; l < 3; l++)
            {
                size_t j = 0;
                while (j < exp_operators.size())
                {
                    if (Parser::get_op_level(exp_operators[j]) == l)
                    {
                        Token *a = buffer->at(j);
                        Token *b = buffer->at(j + 1);
                        OperatorToken *op = new OperatorToken(a->raw + " " + exp_operators[j] + " " + b->raw);
                        // TODO: figure out type in resolve
                        op->op_type = lhc_type::INT8;
                        op->a = new ExpressionToken(a->raw);
                        op->a->content.push_back(a);
                        op->op = exp_operators[j];
                        op->b = new ExpressionToken(b->raw);
                        op->b->content.push_back(b);

                        exp_operators.erase(exp_operators.begin() + j);
                        buffer->at(j) = op;
                        buffer->erase(buffer->begin() + j + 1);
                    }
                    else
                        j++;
                }
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

void Parser::find_end(std::string str, size_t *i, char c)
{
    int level = 0;
    char start = '{';
    if (c == ')')
        start = '(';
    while (*i < str.size() - 1)
    {
        if (str[*i] == start)
            level++;
        else if (str[*i] == c)
        {
            if (level == 0)
            {
                (*i)++;
                return;
            }
            else
                level--;
        }
        (*i)++;
    }
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

err_parse Parser::parse_expression_part(std::string str, std::vector<Token *> *buffer, std::vector<std::string> *exp_operators, size_t *i, std::string *token_buffer)
{
    bool new_token = false;
    bool new_op = false;
    bool parse_var = false;

    if (str[*i] == '"' && str[*i == 0 ? 0 : *i - 1] != '\\')
    {
        size_t start = (*i)++;
        while (!(str[*i] == '"' && str[*i - 1] != '\\') && *i < str.size())
            (*i)++;
        if (*i >= str.size() - 1)
            return err_parse::PARSE_ENDING_STRING;

        buffer->push_back(new LiteralString(str.substr(start, (++*i) - start)));
        new_token = true;
    }
    else if (str[*i] == '\'' && str[*i == 0 ? 0 : *i - 1] != '\\')
    {
        size_t start = (*i)++;
        while (!(str[*i] == '\'' && str[*i - 1] != '\\') && *i < str.size())
            (*i)++;
        if (*i >= str.size() - 1)
            return err_parse::PARSE_ENDING_CHAR;

        buffer->push_back(new LiteralChar(str.substr(start, (++*i) - start)));
        new_token = true;
    }
    else if (CHAR_IS_NUM(str[*i]) || str[*i] == '-')
    {
        std::string prepend = "";
        bool proceed = true;
        if (str[*i] == '-')
        {
            if (*i < str.size() - 1 && CHAR_IS_NUM(str[*i + 1]))
            {
                prepend = "-";
                (*i)++;
            }
            else
            {
                parse_var = true;
                std::string op = str.substr(*i, 2);
                if (STR_IS_OP(op))
                {
                    exp_operators->push_back(op);
                    (*i)++;
                }
                else
                    exp_operators->push_back(op.substr(0, 1));
                new_op = true;
                proceed = false;
            }
        }

        if (proceed)
        {
            if (token_buffer->empty())
            {
                while (CHAR_IS_NUM(str[*i]) && *i < str.size() - 1)
                    token_buffer->push_back(str[(*i)++]);
                if (!CHAR_IS_END(str[*i]) && !CHAR_IS_OP(str[*i]))
                    return err_parse::PARSE_NAN;
                buffer->push_back(new LiteralInt(prepend + *token_buffer));
            }
            else
            {
                if (token_buffer->at(0) == '\'' &&
                    token_buffer->size() > 2 &&
                    token_buffer->back() == '\'' &&
                    token_buffer->at(token_buffer->size() - 2) != '\\')
                {
                    buffer->push_back(new LiteralChar(token_buffer->substr(1, token_buffer->size() - 2)));
                }
                else
                {
                    bool is_num = true;
                    for (size_t j = 0; j < token_buffer->size(); j++)
                    {
                        if (!CHAR_IS_NUM(token_buffer->at(j)) || (j == 0 && token_buffer->at(0) == '-'))
                        {
                            is_num = false;
                            break;
                        }
                    }
                    if (*i < str.size() - 1)
                        (*i)++;
                    while (CHAR_IS_NUM(str[*i]) && *i < str.size() - 1)
                        token_buffer->push_back(str[(*i)++]);

                    if (is_num)
                        buffer->push_back(new LiteralInt(prepend + *token_buffer));
                    else
                        buffer->push_back(new VariableToken(prepend + *token_buffer));
                }
            }

            new_token = true;
        }
        token_buffer->clear();
    }
    else if (CHAR_IS_OP(str[*i]))
    {
        parse_var = true;
        std::string op = str.substr(*i, 2);
        if (STR_IS_OP(op))
        {
            exp_operators->push_back(op);
            (*i)++;
        }
        else
            exp_operators->push_back(op.substr(0, 1));
        new_op = true;
    }
    else // if (CHAR_IS_EMPTY(str[*i]))
        parse_var = true;

    if (parse_var && token_buffer->size() > 0)
    {
        (*i)++;
        while (!CHAR_IS_END(str[*i]) && !CHAR_IS_OP(str[*i]))
            token_buffer->push_back(str[(*i)++]);
        (*i)--; // i is incremented in for loop

        if (token_buffer->at(0) == '\'' &&
            token_buffer->size() > 2 &&
            token_buffer->back() == '\'' &&
            token_buffer->at(token_buffer->size() - 2) != '\\')
        {
            buffer->push_back(new LiteralChar(token_buffer->substr(1, token_buffer->size() - 2)));
        }
        else
        {
            bool is_num = true;
            for (size_t j = 0; j < token_buffer->size(); j++)
            {
                if (!CHAR_IS_NUM(token_buffer->at(j)))
                {
                    is_num = false;
                    break;
                }
            }

            if (is_num)
                buffer->push_back(new LiteralInt(*token_buffer));
            else
                buffer->push_back(new VariableToken(*token_buffer));
        }

        new_token = true;
        token_buffer->clear();
    }

    if (new_token)
    {
        if (buffer->size() != exp_operators->size() + 1)
            return err_parse::PARSE_UNEXPECTED_TOKEN;
    }
    else if (new_op)
    {
        if (buffer->size() != exp_operators->size())
            return err_parse::PARSE_UNEXPECTED_OP;
    }

    return err_parse::PARSE_SUCCESS;
}

int Parser::get_op_level(std::string op)
{
    std::vector<std::vector<std::string>> op_levels = {
        {"*", "/", "%"},
        {"+", "-"},
    };

    for (size_t l = 0; l < op_levels.size(); l++)
        for (size_t i = 0; i < op_levels[l].size(); i++)
            if (op == op_levels[l][i])
                return l;
    return op_levels.size();
}
