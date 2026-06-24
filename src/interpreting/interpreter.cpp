#include <iostream>
#include <string>
#include <format>
#include <cmath>

#include "interpreter.h"
#include "../errors/error.h"

void Interpreter::interpret(const ExprNode& tree) const
{
    try
    {
        tk::Literal result = tree->evaluate();

        std::cout << tk::literal_stringify(result, true) << std::endl;
    }
    catch(const EH::RuntimeError& err)
    {
        EH::runtime_error(err);
    }
}