#pragma once

#include <memory>
#include <string>

#include "../tokenization/token.h"

class Expr
{
public:
    virtual ~Expr() = default; // child classes get their own destructor
    virtual std::string to_string() const = 0; // child classes get their own print methods
};

class Binary : public Expr
{
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

    std::string to_string() const override;
};

class Grouping : public Expr {
public:
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::string to_string() const override;
};

class Literal : public Expr {
public:
    std::any value;
    std::string literal_str;

    Literal(std::any v, std::string ls) 
    : value(v), literal_str(ls) {}

    std::string to_string() const override;
};

class Unary : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}

    std::string to_string() const override;
};
