#pragma once

#include <memory>
#include <string>

#include "../tokenization/token.h"

class Expr
{
public:
    virtual ~Expr() = default; // child classes get their own destructor
    virtual std::string to_string() const = 0; // child classes get their own print methods

    virtual tk::Literal evaluate() const = 0;
};

class BinaryExpr : public Expr
{
private:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

public:
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

    std::string to_string() const override;

    tk::Literal evaluate() const override;
};

class GroupingExpr : public Expr {
private:
    std::unique_ptr<Expr> expression;

public:
    GroupingExpr(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::string to_string() const override;

    tk::Literal evaluate() const override;
};

class LiteralExpr : public Expr {
private:
    tk::Literal value;
    std::string literal_str;

public:
    LiteralExpr(tk::Literal v, std::string ls) 
    : value(v), literal_str(ls) {}

    std::string to_string() const override;
    
    tk::Literal evaluate() const override;
};

class UnaryExpr : public Expr {
private:
    Token op;
    std::unique_ptr<Expr> right;

public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}

    std::string to_string() const override;

    tk::Literal evaluate() const override;
};

bool is_truthy(const tk::Literal& lt);
bool is_equal(const tk::Literal& a, const tk::Literal& b);
void check_double_literals(const tk::Literal& a, const tk::Literal& b, const Token& op);
void check_double_literal(const tk::Literal& a, const Token& op);
