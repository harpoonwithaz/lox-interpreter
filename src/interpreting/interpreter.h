#include <string>

#include "../parsing/parser.h"
#include "../tokenization/token.h"

class Interpreter
{
public:
    void interpret(const StmtList& statements) const;
};