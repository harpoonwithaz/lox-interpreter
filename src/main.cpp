#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// local includes
#include "tokenization/scanner.h"
#include "errors/error.h"
#include "parsing/parser.h"
#include "interpreting/interpreter.h"

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program <command> <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        
        if (!file_contents.empty()) {
            Scanner scanner(file_contents);
            scanner.scan_tokens();
            scanner.print_tokens();
        }
        else
        {
            std::cout << "EOF  null" << std::endl;
        }
        
    }
    else if (command == "parse")
    {
        std::string file_contents = read_file_contents(argv[2]);
        
        if (!file_contents.empty()) {
            Scanner scanner(file_contents);
            scanner.scan_tokens();

            if (EH::had_error) return 65;

            const std::vector<Token> tokens = scanner.get_tokens();
            Parser parser(tokens);
            StmtList stmts = parser.parse();

            if (EH::had_error) return 65;

            parser.print_tree(std::move(stmts));
        }
        else
        {
            std::cout << "EOF  null" << std::endl;
        } 
    }
    else if (command == "evaluate" || command == "run")
    {
        std::string file_contents = read_file_contents(argv[2]);
        
        if (!file_contents.empty()) {
            Scanner scanner(file_contents);
            scanner.scan_tokens();

            if (EH::had_error) return 65;

            const std::vector<Token> tokens = scanner.get_tokens();
            Parser parser(tokens);

            StmtList stmts = parser.parse();

            if (EH::had_error) return 65;

            Interpreter interpreter;
            interpreter.interpret(stmts);

            if (EH::had_runtime_error) return 70;
        }
        else
        {
            std::cout << "EOF  null" << std::endl;
        }
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    if (EH::had_error) return 65;

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
