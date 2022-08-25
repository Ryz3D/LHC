#pragma once

#define CHAR_IS_ALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define CHAR_IS_NUM(c) (c >= '0' && c <= '9')
#define CHAR_IS_ALPHANUM(c) (CHAR_IS_ALPHA(c) || CHAR_IS_NUM(c))
#define CHAR_IS_NAME(c) (CHAR_IS_ALPHANUM(c) || c == '_')
#define CHAR_IS_EMPTY(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define CHAR_IS_END(c) (CHAR_IS_EMPTY(c) || c == ';' || c == ')')
#define CHAR_IS_ASSIGN_OP(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
#define CHAR_IS_OP(c) (CHAR_IS_ASSIGN_OP(c) || c == '<' || c == '>' || c == '|' || c == '&' || c == '=')
#define STR_IS_OP(s) (s == "+" || s == "-" || s == "*" || s == "/" || s == "%" || s == "<" || s == ">" || s == "<=" || s == ">=" || s == "==" || s == "||" || s == "&&")
