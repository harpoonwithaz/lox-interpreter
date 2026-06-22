
#include <memory>
#include <iostream>

#include "parser.h"
#include "token-type.h"

using ExprNode = std::unique_ptr<Expr>;

ExprNode Parser::expression()
{
    return equality();
}

// prasing grammar equality rule:
// equality      -> comparison ( ( "!=" | "==" ) comparison )* ;
ExprNode Parser::equality()
{
    ExprNode expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL}))
    {
        Token op = previous(); // match consumed the token
        ExprNode right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// comparison     -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
ExprNode Parser::comparison()
{
    ExprNode expr = term();

    while (match({
        TokenType::GREATER,
        TokenType::GREATER_EQUAL,
        TokenType::LESS,
        TokenType::LESS_EQUAL
    }))
    {
        Token op = previous(); // match consumed the token
        ExprNode right = term();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprNode Parser::term()
{
    ExprNode expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        ExprNode right = factor();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprNode Parser::factor()
{
    ExprNode expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR}))
    {
        Token op = previous();
        ExprNode right = unary();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprNode Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = previous();
        ExprNode right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }

    return primary();
}

ExprNode Parser::primary()
{
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false, "false");
    if (match({TokenType::TRUE})) return std::make_unique<Literal>(true, "true");
    if (match({TokenType::NIL})) return std::make_unique<Literal>(nullptr, "nil");

    if (match({TokenType::NUMBER, TokenType::STRING})) 
    {
        return std::make_unique<Literal>(previous().get_literal(), previous().get_literal_str());
    }

    if (match({TokenType::LEFT_PAREN}))
    {
        ExprNode expr = expression();
        advance(); // we dont check for right parentheses yet
        return std::make_unique<Grouping>(std::move(expr));
    }

    return nullptr;
}

// ------- Helper methods -------

// checks if current token has any of the given types
bool Parser::match(const std::vector<TokenType>& types)
{
    for (const TokenType& type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }

    return false;
}

// returns true if the current token is of the given type
bool Parser::check(const TokenType& type) const
{
    if (is_at_end()) return false;
    return peek().get_type() == type;
}

// consumes the current token and returns it
Token Parser::advance()
{
    if (!is_at_end()) current++;
    return previous();
}

// checks if we ran out of tokens to parse
bool Parser::is_at_end() const
{
    return peek().get_type() == TokenType::EOFILE;
}

// returns current token we have yet to consume
Token Parser::peek() const
{
    return tokens.at(current);
}

// returns most recently consumed token
Token Parser::previous() const
{
    return tokens.at(current-1);
}


void Parser::print_tree()
{
    ExprNode root = expression();
    if (!root)
    {
        std::cout << "null\n";
    }

    std::cout << root->to_string() << std::endl;
}