#pragma once

#include <vector>
#include <string>

#include "lhc_errors.h"

enum lhc_type
{
    INVALID,
    BOOL,
    UINT8,
    INT8,
    UINT16,
    INT16,
    UINT32,
    INT32,
};

class Token
{
public:
    virtual bool is_supported();
    virtual std::vector<Token *> get_children();
    virtual void resolve();

    bool resolved = false;
    std::string raw = "";
};
