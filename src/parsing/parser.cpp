#include <memory>
#include <iostream>
#include <stdexcept>
#include <variant>

#include "parser.h"
#include "../tokenization/token-type.h"
#include "../errors/error.h"

// Returns the root to the AST of parsed expressions
StmtList Parser::parse()
{
    // old
    // try
    // {
    //     return expression();
    // }
    // catch(const std::exception& e)
    // {
    //     return nullptr;
    // }

    StmtList statements = std::make_unique<std::vector<StmtPtr>>();

    while (!is_at_end())
    {
        statements->push_back(parse_stmt());
    }

    return statements;
}

StmtPtr Parser::parse_stmt()
{
    if (match({TokenType::PRINT})) return print_stmt(); // print stmt

    return expression_stmt();
}



ExprPtr Parser::expression()
{
    return equality();
}

// prasing grammar equality rule:
// equality      -> comparison ( ( "!=" | "==" ) comparison )* ;
ExprPtr Parser::equality()
{
    ExprPtr expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous(); // match consumed the token
        ExprPtr right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// comparison     -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
ExprPtr Parser::comparison()
{
    ExprPtr expr = term();

    while (match({
        TokenType::GREATER,
        TokenType::GREATER_EQUAL,
        TokenType::LESS,
        TokenType::LESS_EQUAL
    }))
    {
        Token op = previous(); // match consumed the token
        ExprPtr right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprPtr Parser::term()
{
    ExprPtr expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        ExprPtr right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprPtr Parser::factor()
{
    ExprPtr expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR}))
    {
        Token op = previous();
        ExprPtr right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprPtr Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = previous();
        ExprPtr right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return primary();
}

ExprPtr Parser::primary()
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
        ExprPtr expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    EH::error(peek(), "Expect expression.");
    throw std::exception(); // to be caught by the parser
}

// Statement operations
StmtPtr Parser::expression_stmt()
{
    ExprPtr expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

StmtPtr Parser::print_stmt()
{
    ExprPtr value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
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

// Advances untill it hits the particular token
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

void Parser::print_tree(StmtList stmts)
{
    // ExprPtr root = expression();
    if (stmts->empty())
    {
        std::cout << "null\n";
        return;
    }

    for (const StmtPtr& stmt : *stmts)
    {
        std::cout << stmt->to_string() << std::endl;
    }
}