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
        case '!':
            add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/'))
            {
                // A comment goes until the end of the line
                while (peek() != '\n' && !is_at_end()) current++;
            }
            else
            {
                add_token(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ingore whitespace, enter, carriage return
            break;

        case '\n':
            line++;
            break;

        case '"': 

        default: 
            std::string msg = "Unexpected character: ";
            msg.push_back(c);
            EH::error(line, msg);
            break;
    }
}

void Scanner::string()
{
    while (peek() != '"' && !is_at_end())
    {
        if (peek() == '\n') line++; // multiline strings
        current++;
    }

    if (is_at_end())
    {
        EH::error(line, "Unterminated string.");
        return;
    }

    // the closing quote
    current++;

    // exclude the beginning and end quotes
    std::string value = source.substr(start+1, (current-start)-1);
    add_token(TokenType::STRING, value);
}

bool Scanner::match(char expected)
{
    if (is_at_end()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

// Returns the current char in the source
char Scanner::peek() const
{
    if (is_at_end()) return '\0'; // null terminator
    return source[current];
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