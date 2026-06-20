#include "error-handling.h"
#include <stddef.h>
#include <string>

void EH::report(size_t line, std::string where, std::string message)
{
    std::string msg = "[line " + std::to_string(line) + "] Error" + where + ":" + message;
    EH::had_error = true;
}

void EH::error(size_t line, std::string message)
{
    report(line, "", message);
}