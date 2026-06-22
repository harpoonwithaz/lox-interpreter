#pragma once

#include <vector>
#include <cstddef>
#include <memory>

#include "token.h"
#include "expr.h"

using ExprNode = std::unique_ptr<Expr>;

class Parser
{
private:
    const std::vector<Token> tokens;
    size_t current;

    // Binary operations
    ExprNode expression();
    ExprNode equality();
    ExprNode comparison();
    ExprNode term();
    ExprNode factor();

    ExprNode unary();
    ExprNode primary();

    // Helper methods
    bool match(const std::vector<TokenType>& types);
    bool check(const TokenType& type) const;
    Token advance();
    bool is_at_end() const;
    Token peek() const;
    Token previous() const;
public:
    Parser(const std::vector<Token>& t): tokens(t), current(0) {}

    void print_tree();
};