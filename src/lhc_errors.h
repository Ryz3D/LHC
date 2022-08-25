#pragma once

enum err_parse
{
    PARSE_SUCCESS,
    PARSE_ENDING_STRING,            // unclosed string
    PARSE_ENDING_CHAR,              // unclosed char
    PARSE_ENDING_ASSIGNMENT,        // incomplete assignment
    PARSE_UNEXPECTED_TYPE,          // type keyword at illegal position
    PARSE_NAN,                      // numeric literal not terminated with CHAR_END or CHAR_OP (see defines.h)
    PARSE_UNEXPECTED_FUNCTION_DEF,  // function definition at illegal position
    PARSE_UNEXPECTED_FUNCTION_CALL, // function call at illegal position
    PARSE_UNEXPECTED_STATEMENT,     // if, while, for at illegal position
    PARSE_MULTI_DEF,                // comma after definition
    PARSE_UNEXPECTED_TOKEN,         // expected operator after token, got another token
    PARSE_UNEXPECTED_OP             // expected expression after operator, got another operator
};

enum err_resolve
{
    RESOLVE_SUCCESS,
    RESOLVE_NOT_SUPPORTED,
    RESOLVE_MISSING
};

enum err_compile
{
    COMPILE_SUCCESS,
    COMPILE_NO_MAIN,     // no main function found
    COMPILE_NO_EXIT,     // main function does not end with return
    COMPILE_UNDEF_VAR,   // undefined variable
    COMPILE_UNDEF_FUNC,  // undefined function
    COMPILE_MISSING_ARG, // missing expected argument in function call
    COMPILE_MISSING_EXP, // missing expected expression
    COMPILE_ILLEGAL_OP   // unknown operator
};

enum err_assemble
{
    ASSEMBLE_SUCCESS,
    ASSEMBLE_UNDEF_LABEL,  // no label found for label literal
    ASSEMBLE_RAM_OVERFLOW, // TODO
    ASSEMBLE_ROM_OVERFLOW  // TODO
};
