#pragma once

#include <string>

#include "expr.h"

class Stmt
{
public:
    virtual ~Stmt() = default;

    virtual void execute() const = 0;
    virtual std::string to_string() const = 0;
};

class ExpressionStmt : public Stmt
{
private:
    std::unique_ptr<Expr> expr;
public:
    ExpressionStmt(std::unique_ptr<Expr> expr): expr(std::move(expr)) {}

    void execute() const override;

    std::string to_string() const override;
};

class PrintStmt : public Stmt
{
private:
    std::unique_ptr<Expr> expr;
public:
    PrintStmt(std::unique_ptr<Expr> value): expr(std::move(value)) {}

    void execute() const override;

    std::string to_string() const override;
};