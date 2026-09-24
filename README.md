# lox-interpreter

An interpreter for the [Lox language](https://craftinginterpreters.com/) based on Bob Nystrom's *Crafting Interpreters*.

## Features

- [x] **Lexical Analysis (Scanning):** Converts raw Lox source code into structured tokens with location tracking and error reporting.
- [x] **Syntactic Analysis (Parsing):** Recursive descent parser generating an Abstract Syntax Tree (AST).
- [x] **Execution (Tree-Walk / Bytecode):** Evaluates expressions, (WIP: statements, control flow, functions, closures, and classes).
- [x] **REPL & Script Execution:** Interactive prompt for rapid evaluation or direct execution of `.lox` source files.

## Prerequisites

- Toolchain: `C++`
- Build tool: `cmake`
