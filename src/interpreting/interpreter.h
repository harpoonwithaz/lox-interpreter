#include <string>

#include "../parsing/expr.h"
#include "../parsing/parser.h"
#include "../tokenization/token.h"

class Interpreter
{
public:
    void interpret(const ExprNode& tree) const;
};