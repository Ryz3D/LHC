# The _Large Headache Compiler_

[![CI](https://github.com/Ryz3D/LHC/actions/workflows/main.yml/badge.svg)](https://github.com/Ryz3D/LHC/actions/workflows/main.yml)

This project generates assembly and machine code for a little computer with a very limited instruction set. As a source it targets a subset of C, so any program can also be compiled using gcc and run on a _proper_ computer and hopefully that yields the same result.
I fully intend to build [a machine](https://github.com/Ryz3D/SPEAR) capable of executing the generated binaries in hardware, but time will tell if I'm actually able to.

## Implemented

- Parsing .c files
- Basic variables (`uint_8`, `int_8`, `bool`)
  - All variables are treated as global
- Operators
  - `+`, `-`, `*`, `/` also as assignment: `+=`, `-=`, `*=`, `/=` and `++`, `--`
  - `<`, `>`, `<=`, `>=`, `==`, `!=`, `||`, `&&`
- `if` and `else` by conditional jumps
- `while` by conditional and fixed jumps
- `for` by transformation to `while`
- Basic functions
  - `putchar` writes to RAM 0x07
  - `getchar` reads from RAM 0x07
  - `printf` for string literal only
  - `print_int` for printing signed numbers
  - `delay` keeps CPU busy for specified number of clock cycles
- Custom CPU simulator (and brainfuck, just for fun)
- Output, parse and execute custom assembly
  - Can be used as an assembly debugger/simulator and assembly-to-binary converter
- Output as binary and Logisim image

## TO-DO

- Paranthesis in expression
- Division/modulo by zero handler
- Multi-byte types
- Native `print_uint` function > 127

## _(notes for version two)_

- Simpler tokenization, seperate from parsing
  - Only keywords, names and symbols
  - ExpressionToken is useless
  - Use `struct` for Token types
    - ~I could write it in C...~ **don't**
- Proper error messages (show problematic line/keyword)
- Proper functions
  - Heap, stack
