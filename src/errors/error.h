#pragma once

#include <stddef.h>
#include <string>

#include "../tokenization/token.h"

namespace EH
{
    inline bool had_error = false;
    
    void report(size_t line, std::string where, std::string message);
    void error(size_t line, std::string message);
    void error(Token token, std::string message);
}
