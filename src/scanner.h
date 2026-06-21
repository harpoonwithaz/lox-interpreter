#pragma once

#include "token.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

class Scanner
{
private:
    const std::string source;
    std::unique_ptr<std::vector<Token>> tokens; 

    size_t start;
    size_t current;
    size_t line;

    std::unordered_map<std::string, TokenType> keywords =
    {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
    };

    bool is_at_end() const;
    void scan_token();
    void identifier();
    void number();
    void string();
    bool match(char expected);
    char peek() const;
    char peek_next() const;
    bool is_alpha(char c) const;
    bool is_alpha_numeric(char c) const;
    bool is_digit(char c) const;
    void add_token(const TokenType& type);
    void add_token(const TokenType& type, const std::any& literal);
public:
    inline Scanner(std::string source)
    : 
    source(source), 
    tokens(std::make_unique<std::vector<Token>>()),
    start(0), current(0), line(1) {};

    void scan_tokens();
    void print_tokens() const;
};