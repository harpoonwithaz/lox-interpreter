#include <string>

#include "expr.h"

// Returns a string representation of the subtree
std::string Binary::to_string() const
{
    std::string out;
    out.push_back('(');
    out.append(op.get_lexeme());
    out.push_back(' ');
    out.append(left ? left->to_string() : "nil");
    out.push_back(' ');
    out.append(right ? right->to_string() : "nil");
    out.push_back(')');
    return out;
}

std::string Grouping::to_string() const
{
    std::string out;
    out.push_back('(');
    out.append("group ");
    out.append(expression ? expression->to_string() : "nil");
    out.push_back(')');
    return out;
}

// leaf nodes, so we just return the str representation
std::string Literal::to_string() const { return literal_str; }

std::string Unary::to_string() const
{
    std::string out;
    out.push_back('(');
    out.append(op.get_lexeme());
    out.push_back(' ');
    out.append(right ? right->to_string() : "nil");
    out.push_back(')');
    return out;
}

