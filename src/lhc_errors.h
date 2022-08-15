#pragma once

enum err_parse
{
    PARSE_SUCCESS,
    PARSE_ENDING_STRING,            // unclosed string
    PARSE_ENDING_CHAR,              // unclosed char
    PARSE_ENDING_ASSIGNMENT,        // incomplete assignment
    PARSE_UNEXPECTED_TYPE,          // type keyword at illegal position
    PARSE_NAN,                      // numeric literal not terminated with ' ', ';' or ')'
    PARSE_UNEXPECTED_FUNCTION_DEF,  // function definition at illegal position
    PARSE_UNEXPECTED_FUNCTION_CALL, // function call at illegal position
    PARSE_MULTI_DEF                 // comma after definition
};

enum err_resolve
{
    RESOLVE_SUCCESS,
    RESOLVE_NOT_SUPPORTED,
    RESOLVE_MISSING
};

enum err_compile
{
    COMPILE_SUCCESS
};

enum err_assemble
{
    ASSEMBLE_SUCCESS
};
