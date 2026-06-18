#include "token-type.h"

#include <string>
#include <any>



class Token
{
private:
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const size_t line;

public:
    Token(TokenType type, std::string lexeme, std::any literal, size_t line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

    std::string to_string() const
    {
        return type + " " + lexeme + " ";
    }
};