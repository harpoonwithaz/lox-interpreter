#pragma once

#include <vector>
#include <cstddef>
#include <memory>

#include "expr.h"
#include "stmt.h"
#include "../tokenization/token.h"

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;

using StmtList = std::unique_ptr<std::vector<StmtPtr>>;

class Parser
{
private:
    const std::vector<Token> tokens;
    size_t current;

    // Expression operations
    ExprPtr expression();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();

    ExprPtr unary();
    ExprPtr primary();

    // Statement operations
    StmtPtr expression_stmt();
    StmtPtr print_stmt();

    // Helper methods
    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    bool check(const TokenType& type) const;
    Token advance();
    bool is_at_end() const;
    Token peek() const;
    Token previous() const;

    void synchronize();
public:
    Parser(const std::vector<Token>& t): tokens(t), current(0) {}

    StmtList parse();
    StmtPtr parse_stmt();

    void print_tree(StmtList stmts);
};