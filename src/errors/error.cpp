#include <stddef.h>
#include <string>
#include <iostream>
#include <format>

#include "error.h"
#include "../tokenization/token.h"

void EH::report(size_t line, const std::string& where, const std::string& message)
{
    std::string msg = std::format("[line {}] Error{}: {}", line, where, message);
    std::cerr << msg << std::endl;
    EH::had_error = true;
}

void EH::error(size_t line, const std::string& message)
{
    report(line, "", message);
}

void EH::error(Token token, const std::string& message)
{
    if (token.get_type() == TokenType::EOFILE) report(token.get_line(), " at end", message);
    else report(token.get_line(), " at '" + token.get_lexeme() + "'", message);
}

void EH::runtime_error(const RuntimeError& err)
{
    std::string msg = std::format("{} \n[line {}]", err.what(), err.token.get_line());
    std::cerr << msg << std::endl;
    EH::had_runtime_error = true; 
}