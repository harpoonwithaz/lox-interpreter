#include <iostream>
#include <string>
#include <format>
#include <cmath>

#include "interpreter.h"
#include "../errors/error.h"

std::string Interpreter::stringify(const tk::Literal& lt) const
{
    if (std::holds_alternative<std::monostate>(lt)) return "nil";
    if (std::holds_alternative<std::string>(lt)) return std::get<std::string>(lt);
    if (std::holds_alternative<bool>(lt)) return (std::get<bool>(lt) ? "true" : "false"); 
    if (std::holds_alternative<double>(lt))
    {
        double value = std::get<double>(lt);

        std::string formatted_val;
        if (value == std::floor(value))
        {
            // If it's a whole number, strip off 0
            formatted_val = std::format("{:.0f}", value);
        }
        else
        {
            // Otherwise, let std::format dynamically print the exact precision without trailing zeros
            formatted_val = std::format("{}", value);
        }
        
        return formatted_val;
    }

    return "nil";
}

void Interpreter::interpret(const ExprNode& tree) const
{
    try
    {
        tk::Literal result = tree->evaluate();

        std::cout << stringify(result) << std::endl;
    }
    catch(const EH::RuntimeError& err)
    {
        EH::runtime_error(err);
    }
}