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

    size_t start;
    size_t current;
    size_t line;

    bool is_at_end() const { return current >= source.length(); }
    
    void scan_token()
    {
        char c = source[current++];
    
        switch (c)
        {
            case '(': add_token(LEFT_PAREN); break;
            case ')': add_token(RIGHT_PAREN); break;
            case '{': add_token(LEFT_BRACE); break;
            case '}': add_token(RIGHT_BRACE); break;
            case ',': add_token(COMMA); break;
            case '.': add_token(DOT); break;
            case '-': add_token(MINUS); break;
            case '+': add_token(PLUS); break;
            case ';': add_token(SEMICOLON); break;
            case '*': add_token(STAR); break; 
        }
    }
    
    void add_token(const TokenType& type) { add_token(type, nullptr); }
    
    void add_token(const TokenType& type, const std::any& literal)
    {
        std::string text = source.substr(start, (current-start)+1);
        tokens->push_back(Token(type, "text", literal, line));
    }
public:
    Scanner(std::string source)
    : 
    source(source), 
    tokens(std::make_unique<std::vector<Token>>()),
    start(0), current(0), line(1) {}

    void scan_tokens()
    {
        while (!is_at_end())
        {
            start = current;
            scan_token();
        }

        // add EOF token
        tokens->push_back(Token(EOFILE, "", nullptr, line));
    }

    void print_tockens() const
    {
        for (const Token& t : *tokens)
        {
            std::cout << t.to_string() << std::endl;   
        }
    }

};