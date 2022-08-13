#pragma once

enum err_parse
{
    PARSE_SUCCESS,
    PARSE_ENDING_STRING,
    PARSE_ENDING_CHAR
};

enum err_resolve
{
    RESOLVE_SUCCESS,
    RESOLVE_NOT_SUPPORTED,
    RESOLVE_MISSING
};

enum err_assemble
{
    ASSEMBLE_SUCCESS
};
