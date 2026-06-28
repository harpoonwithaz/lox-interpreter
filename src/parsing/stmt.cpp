#include <iostream>

#include "stmt.h"

void ExpressionStmt::execute() const
{
    expr->evaluate();
}

void PrintStmt::execute() const
{
    tk::Literal literal = expr->evaluate();
    std::cout << tk::literal_stringify(literal, true) << std::endl;
}

std::string ExpressionStmt::to_string() const { return expr->to_string(); }

std::string PrintStmt::to_string() const { return expr->to_string(); }