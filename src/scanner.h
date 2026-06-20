#pragma once

#include "token.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

class Scanner
{
private:
    const std::string source;
    std::unique_ptr<std::vector<Token>> tokens; 

    size_t start;
    size_t current;
    size_t line;

    bool is_at_end() const;
    void scan_token();
    void string();
    bool match(char expected);
    char peek();
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