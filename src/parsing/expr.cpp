#include <string>
#include <variant>

#include "expr.h"
#include "../errors/error.h"

// We use regular inheritence rather than the vistor model from the textbook
// since this is c++ and not java plus the visitor model adds too much
// boilerplate and it just a stupid concept that I can't understand

tk::Literal BinaryExpr::evaluate() const
{
    tk::Literal left_val = left->evaluate();
    tk::Literal right_val = right->evaluate();

    switch (op.get_type())
    {
        case TokenType::GREATER:
        {
            check_double_literals(left_val, right_val, op);
            
            return std::get<double>(left_val) > std::get<double>(right_val);
        }

        case TokenType::GREATER_EQUAL:
        {
            check_double_literals(left_val, right_val, op);
    
            return std::get<double>(left_val) >= std::get<double>(right_val);
        }

        case TokenType::LESS:
        {
            check_double_literals(left_val, right_val, op);
    
            return std::get<double>(left_val) < std::get<double>(right_val);
        }

        case TokenType::LESS_EQUAL:
        {
            check_double_literals(left_val, right_val, op);
    
            return std::get<double>(left_val) <= std::get<double>(right_val);
        }

        case TokenType::MINUS:
        {
            check_double_literals(left_val, right_val, op);

            // dynamic type conversion
            return std::get<double>(left_val) - std::get<double>(right_val);
        }

        case TokenType::SLASH:
        {
            check_double_literals(left_val, right_val, op);

            double divisor = std::get<double>(right_val);
            if (divisor == 0) throw EH::RuntimeError(op, "Cannot divide by zero.");

            return std::get<double>(left_val) / divisor;
        }

        case TokenType::STAR:
        {
            check_double_literals(left_val, right_val, op);

            return std::get<double>(left_val) * std::get<double>(right_val);
        }

        case TokenType::PLUS:
        {
            if (std::holds_alternative<double>(left_val) &&
                std::holds_alternative<double>(right_val))
            {
                
                return std::get<double>(left_val) + std::get<double>(right_val);
            }
            else if (std::holds_alternative<std::string>(left_val) &&
                    std::holds_alternative<std::string>(right_val))
            {
                // lox's string concatenation
                return std::get<std::string>(left_val) + std::get<std::string>(right_val);
            }
            else
            {
                // its not really trivial how to know if they wanted to add strings or numbers
                // so ill just say that they have to both be the same
                throw EH::RuntimeError(op, "Operands must both be numbers or strings.");
            }
        }

        case TokenType::BANG_EQUAL: return !is_equal(left_val, right_val);

        case TokenType::EQUAL_EQUAL: return is_equal(left_val, right_val);
        
        default: return std::monostate{};
    }

    return std::monostate{}; // so compiler will stfu (unreachable)
}

tk::Literal GroupingExpr::evaluate() const
{
    return expression->evaluate();
}

tk::Literal LiteralExpr::evaluate() const
{
    return value;
}

tk::Literal UnaryExpr::evaluate() const
{
    // evaluate right node first
    tk::Literal right_val = right->evaluate();

    switch (op.get_type())
    {
    case TokenType::BANG: return !is_truthy(right_val);
    case TokenType::MINUS:
        check_double_literal(right_val, op);

        return -std::get<double>(right_val);
    default:
        break;
    }

    return std::monostate{}; // so compiler will stfu (unreachable)
}

// helper functions
bool is_truthy(const tk::Literal& lt)
{
    // in lox null and false is falsy, everything else is truthy
    if (std::holds_alternative<std::monostate>(lt)) return false;
    if (std::holds_alternative<bool>(lt)) return std::get<bool>(lt);
    
    return true;
}

bool is_equal(const tk::Literal& a, const tk::Literal& b)
{
    // idk what edge cases these are, the textbook just adds them
    if (std::holds_alternative<std::monostate>(a) &&
        std::holds_alternative<std::monostate>(b)) return true;
    if (std::holds_alternative<std::monostate>(a)) return false;

    return a == b;
}

// Throws a runtime error if either literal cannot be cast to a double
void check_double_literals(const tk::Literal& a, const tk::Literal& b, const Token& op)
{
    if (!std::holds_alternative<double>(a) ||
        !std::holds_alternative<double>(b))
    {
        throw EH::RuntimeError(op, "Operands must be numbers.");
    }
}

void check_double_literal(const tk::Literal& a, const Token& op)
{
    if (!std::holds_alternative<double>(a))
    {
        throw EH::RuntimeError(op, "Operand must be a number.");
    }
}

// For printing
// Returns a string representation of the subtree
std::string BinaryExpr::to_string() const
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

std::string GroupingExpr::to_string() const
{
    std::string out;
    out.push_back('(');
    out.append("group ");
    out.append(expression ? expression->to_string() : "nil");
    out.push_back(')');
    return out;
}

// leaf nodes, so we just return the str representation
std::string LiteralExpr::to_string() const { return literal_str; }

std::string UnaryExpr::to_string() const
{
    std::string out;
    out.push_back('(');
    out.append(op.get_lexeme());
    out.push_back(' ');
    out.append(right ? right->to_string() : "nil");
    out.push_back(')');
    return out;
}

