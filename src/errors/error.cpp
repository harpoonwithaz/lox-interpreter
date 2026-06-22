#include "error.h"

#include <stddef.h>
#include <string>
#include <iostream>

void EH::report(size_t line, std::string where, std::string message)
{
    std::string msg = "[line " + std::to_string(line) + "] Error" + where + ": " + message;
    std::cerr << msg << std::endl;
    EH::had_error = true;
}

void EH::error(size_t line, std::string message)
{
    report(line, "", message);
}