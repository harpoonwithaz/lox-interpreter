#include <iostream>
#include <string>
#include <format>
#include <cmath>

#include "interpreter.h"
#include "../errors/error.h"

void Interpreter::interpret(const StmtList& statements) const
{
    try
    {
        // tk::Literal result = tree->evaluate();

        // std::cout << tk::literal_stringify(result, true) << std::endl;
        for (const StmtPtr& stmt : *statements)
        {
            stmt->execute();
        }
    }
    catch(const EH::RuntimeError& err)
    {
        EH::runtime_error(err);
    }
}