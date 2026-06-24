#include <string>

#include "../parsing/expr.h"
#include "../parsing/parser.h"
#include "../tokenization/token.h"

class Interpreter
{
private:
    std::string stringify(const tk::Literal& lt) const;
public:
    void interpret(const ExprNode& tree) const;
};