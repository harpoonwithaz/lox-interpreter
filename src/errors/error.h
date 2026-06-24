#pragma once

#include <stddef.h>
#include <string>
#include <stdexcept>

#include "../tokenization/token.h"

namespace EH
{
    inline bool had_error = false;
    inline bool had_runtime_error = false;

    struct RuntimeError : public std::runtime_error
    {
        Token token;
        RuntimeError(Token t, const std::string& msg)
        : std::runtime_error(msg), token(t) {}
    };
    
    void report(size_t line, const std::string& where, const std::string& message);
    void error(size_t line, const std::string& message);
    void error(Token token, const std::string& message);
    void runtime_error(const RuntimeError& err);
}
