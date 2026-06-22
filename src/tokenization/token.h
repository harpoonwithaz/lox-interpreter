#pragma once

#include "token-type.h"

#include <string>
#include <any>

class Token
{
private:
    const TokenType type;
    const std::string type_str; // for printing
    const std::string lexeme;
    const std::any literal;
    const std::string literal_str; // for printing
    const size_t line;

    std::string type_string(TokenType t);
    std::string literal_string(const std::any& literal);
public:
    inline Token(TokenType type, std::string lexeme, std::any literal, size_t line)
    : type(type), type_str(type_string(type)), lexeme(lexeme),
    literal(literal), literal_str(literal_string(literal)), line(line) {}

    std::string to_string() const;

    TokenType get_type() const;
    std::any get_literal() const;
    const std::string& get_lexeme() const;
    const std::string& get_literal_str() const;
    size_t get_line() const;
};