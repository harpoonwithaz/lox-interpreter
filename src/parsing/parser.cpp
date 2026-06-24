#include <memory>
#include <iostream>
#include <stdexcept>
#include <variant>

#include "parser.h"
#include "../tokenization/token-type.h"
#include "../errors/error.h"

// Returns the root to the AST of parsed expressions
ExprNode Parser::parse()
{
    try
    {
        return expression();
    }
    catch(const std::exception& e)
    {
        return nullptr;
    }
    
}

ExprNode Parser::expression()
{
    return equality();
}

// prasing grammar equality rule:
// equality      -> comparison ( ( "!=" | "==" ) comparison )* ;
ExprNode Parser::equality()
{
    ExprNode expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous(); // match consumed the token
        ExprNode right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
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
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
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
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
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
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprNode Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = previous();
        ExprNode right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return primary();
}

ExprNode Parser::primary()
{
    if (match({TokenType::FALSE})) return std::make_unique<LiteralExpr>(false, "false");
    if (match({TokenType::TRUE})) return std::make_unique<LiteralExpr>(true, "true");
    if (match({TokenType::NIL})) return std::make_unique<LiteralExpr>(std::monostate{}, "nil");

    if (match({TokenType::NUMBER, TokenType::STRING})) 
    {
        return std::make_unique<LiteralExpr>(previous().get_literal(), previous().get_literal_str());
    }

    if (match({TokenType::LEFT_PAREN}))
    {
        ExprNode expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    EH::error(peek(), "Expect expression.");
    throw std::exception(); // to be caught by the parser
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

Token Parser::consume(TokenType type, const std::string& message)
{
    if (check(type)) return advance();

    EH::error(peek(), message);
    throw std::exception(); // to be caught by the parser
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

void Parser::synchronize()
{
    advance();

    while (!is_at_end())
    {
        if (previous().get_type() == TokenType::SEMICOLON) return; // end of the statement

        switch (peek().get_type())
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}

void Parser::print_tree(ExprNode root)
{
    // ExprNode root = expression();
    if (!root)
    {
        std::cout << "null\n";
    }

    std::cout << root->to_string() << std::endl;
}