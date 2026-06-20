#pragma once

#include "token-type.h"

#include <string>
#include <any>

class Token
{
private:
    const TokenType type;
    const std::string type_str;
    const std::string lexeme;
    const std::any literal;
    const size_t line;

    std::string type_string(TokenType t);
public:
    inline Token(TokenType type, std::string lexeme, std::any literal, size_t line)
    : type(type), type_str(type_string(type)), lexeme(lexeme), literal(literal), line(line) {}

    std::string to_string() const;
};