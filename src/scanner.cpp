#include "token.cpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

class Scanner
{
private:
    const std::string source;
    std::unique_ptr<std::vector<Token>> tokens; 
    // std::vector<Token> tokens;

    size_t start;
    size_t current;
    size_t line;

    bool is_at_end() const { return current >= source.length(); }
    
    void scan_token()
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
        }
    }
    
    void add_token(const TokenType& type) { add_token(type, nullptr); }
    
    void add_token(const TokenType& type, const std::any& literal)
    {
        std::string text = source.substr(start, (current-start));
        tokens->push_back(Token(type, text, literal, line));
        // tokens.push_back(Token(type, text, literal, line));
    }
public:
    Scanner(std::string source)
    : 
    source(source), 
    tokens(std::make_unique<std::vector<Token>>()),
    // tokens{},
    start(0), current(0), line(1) {}

    void scan_tokens()
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

    void print_tockens() const
    {
        for (const Token& t : *tokens)
        // for (const Token& t : tokens)
        {
            std::cout << t.to_string() << std::endl;   
        }
    }

};