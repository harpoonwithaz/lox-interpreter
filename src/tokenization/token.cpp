#include <string>
#include <variant>
#include <cstddef>
#include <format>
#include <cmath>

#include "token-type.h"
#include "token.h"

// why doesn't c++ have a way to print the name of the enum :(
std::string Token::type_string(TokenType t) const
{
    switch (t)
    {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::MINUS: return "MINUS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::SLASH: return "SLASH";
        case TokenType::STAR: return "STAR";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::AND: return "AND";
        case TokenType::CLASS: return "CLASS";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FUN: return "FUN";
        case TokenType::FOR: return "FOR";
        case TokenType::IF: return "IF";
        case TokenType::NIL: return "NIL";
        case TokenType::OR: return "OR";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::SUPER: return "SUPER";
        case TokenType::THIS: return "THIS";
        case TokenType::TRUE: return "TRUE";
        case TokenType::VAR: return "VAR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::EOFILE: return "EOF";

        default: return "";
    }
}

// helper function that casts dynamic lox type to string representation and returns it
std::string tk::literal_stringify(const Literal& lt, bool lox)
{
    if (std::holds_alternative<std::monostate>(lt)) return lox ? "nil" : "null";
    if (std::holds_alternative<std::string>(lt)) return std::get<std::string>(lt);
    if (std::holds_alternative<bool>(lt)) return (std::get<bool>(lt) ? "true" : "false"); 
    if (std::holds_alternative<double>(lt))
    {
        double value = std::get<double>(lt);

        std::string formatted_val;
        if (value == std::floor(value))
        {
            // If it's a whole number, force exactly 1 decimal place (.0)
            // If we want the lox representation, we use 0 decimal places for whole numbers
            formatted_val = lox ? std::format("{:.0f}", value) : std::format("{:.1f}", value);
        }
        else
        {
            // Otherwise, let std::format dynamically print the exact precision without trailing zeros
            formatted_val = std::format("{}", value);
        }
        
        return formatted_val;
    }

    return lox ? "nil" : "null";
}

std::string Token::to_string() const
{
    return std::format("{} {} {}", type_str, lexeme, literal_str);
}

TokenType Token::get_type() const { return type; }

tk::Literal Token::get_literal() const { return literal; }

const std::string& Token::get_lexeme() const { return lexeme; }

const std::string& Token::get_literal_str() const { return literal_str; }

size_t Token::get_line() const { return line; }