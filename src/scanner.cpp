#include "token.h"
#include "scanner.h"
#include "error.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <format>

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

        case '"': string(); break;

        default: 
            if (is_digit(c)) number();
            else if (is_alpha(c)) identifier();
            else
            {
                std::string msg = "Unexpected character: ";
                msg.push_back(c);
                EH::error(line, msg);
            }
            break;
    }
}

void Scanner::identifier()
{
    while (is_alpha_numeric(peek())) current++;

    add_token(TokenType::IDENTIFIER);
}

void Scanner::number()
{
    while (is_digit(peek())) current++;

    // decimal points
    if (peek() == '.' && is_digit(peek_next()))
    {
        // consume the '.'
        current++;

        while (is_digit(peek())) current++;
    }

    try
    {
        double value = std::stod(source.substr(start, (current-start)));
        add_token(TokenType::NUMBER, value);
    }
    catch(const std::exception& e)
    {
        std::string msg = std::format("Failed to convert number. {}", e.what());
        EH::error(line, msg); // will make it more verbose later
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
    std::string value = source.substr(start+1, (current-start)-2);
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

char Scanner::peek_next() const
{
    if (current + 1 >= source.length()) return '\0';
    return source[current+1];
}

bool Scanner::is_alpha(char c) const
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
             c == '_');
}

bool Scanner::is_alpha_numeric(char c) const
{
    return (is_alpha(c) || is_digit(c));
}

bool Scanner::is_digit(char c) const { return (c >= '0' && c <= '9'); }

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