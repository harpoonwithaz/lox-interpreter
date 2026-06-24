#pragma once

#include <string>
#include <variant>

#include "token-type.h"

// Type definition for literals using std::variant
// std::monostate is how we will represent lox's 'nil' type
namespace tk 
{ 
    using Literal = std::variant<std::monostate, std::string, bool, double>; 

    std::string literal_stringify(const Literal& lt, bool lox);
}

class Token
{
private:
    const TokenType type;
    const std::string type_str; // for printing
    const std::string lexeme;
    const tk::Literal literal;
    const std::string literal_str; // for printing
    const size_t line;

    std::string type_string(TokenType t) const;
public:
    inline Token(TokenType t, std::string lx, tk::Literal lt, size_t ln)
    : 
    type(t),
    type_str(type_string(t)),
    lexeme(lx),
    literal(lt),
    literal_str(tk::literal_stringify(lt, false)),
    line(ln) {}

    std::string to_string() const;

    TokenType get_type() const;
    tk::Literal get_literal() const;
    const std::string& get_lexeme() const;
    const std::string& get_literal_str() const;
    size_t get_line() const;
};