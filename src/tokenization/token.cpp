#include "token-type.h"
#include "token.h"

#include <string>
#include <any>
#include <iostream>
#include <cstddef>
#include <format>
#include <cmath>

std::string Token::type_string(TokenType t)
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

// std::any is black magic and i dont really care how it works, only that it does
std::string Token::literal_string(const std::any& literal)
{
    if (!literal.has_value()) return "null";

    if (literal.type() == typeid(std::nullptr_t)) return "null";
    if (literal.type() == typeid(std::string)) return std::any_cast<const std::string&>(literal);
    if (literal.type() == typeid(double)) 
    {
        double value = std::any_cast<double>(literal);

        std::string formatted_val;
        if (value == std::floor(value)) {
            // If it's a whole number, force exactly 1 decimal place (.0)
            formatted_val = std::format("{:.1f}", value);
        } else {
            // Otherwise, let std::format dynamically print the exact precision without trailing zeros
            formatted_val = std::format("{}", value);
        }
        
        return formatted_val;
    }

    return "null";
}

std::string Token::to_string() const
{
    return std::format("{} {} {}", type_str, lexeme, literal_str);
}

TokenType Token::get_type() const { return type; }

std::any Token::get_literal() const { return literal; }

const std::string& Token::get_lexeme() const { return lexeme; }

const std::string& Token::get_literal_str() const { return literal_str; }