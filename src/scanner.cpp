#include "token.h"
#include "scanner.h"
#include "error.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>

bool Scanner::is_at_end() const { return current >= source.length(); }

void Scanner::scan_token()
{
    char c = source[current++];

    switch (c)
    {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::STAR); break; 

        default: 
            std::string msg = "Unexpected character: " + std::to_string(c);
            EH::error(line, msg);
            break;
    }
}

void Scanner::add_token(const TokenType& type) { add_token(type, nullptr); }

void Scanner::add_token(const TokenType& type, const std::any& literal)
{
    std::string text = source.substr(start, (current-start));
    tokens->push_back(Token(type, text, literal, line));
}

void Scanner::scan_tokens()
{
    while (!is_at_end())
    {
        start = current;
        scan_token();
    }

    // add EOF token
    tokens->push_back(Token(TokenType::EOFILE, "", nullptr, line));
    // tokens.push_back(Token(TokenType::EOFILE, "", nullptr, line));
}

void Scanner::print_tokens() const
{
    for (const Token& t : *tokens)
    // for (const Token& t : tokens)
    {
        std::cout << t.to_string() << std::endl;   
    }
}