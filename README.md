# The _Large Headache Compiler_

This project generates assembly and machine code for a little computer with a very limited instruction set. As a source it targets a subset of C, so any program can also be compiled using gcc and run on a _proper_ computer and hopefully that yields the same result.
I fully intend to build a machine capable of executing the generated binaries in hardware, but time will tell if I'm actually able to.

## Implemented

_(actually)_

- Parsing .c files
- Basic variables (`uint_8`, `int_8`, `bool`)
  - All variables are treated as global for now
- Operators
  - `+`, `-`, `*`, `/` also as assignment: `+=`, `-=`, `*=`, `/=` and `++`, `--`
  - `<`, `>`, `<=`, `>=`, `==`, `!=`, `||`, `&&`
- `if` by conditional jumps
- `while` by conditional and fixed jumps
- `for` by transformation to `while`
- Basic functions
  - `putchar` writes to RAM 0x05
  - `getchar` reads from RAM 0x05
  - `printf` for string literal only
- Custom CPU simulator (and brainfuck, just for fun)
- Output, parse and execute custom assembly
  - Can be used as an assembly debugger/simulator and assembly->binary converter
- Output as binary (for flash memory and LogiSim)

## TO-DO

- Handle `else` by jumping past by default
- Multi-byte types
- Native `print_uint` function for multiple digits
- Native `print_int` function for negative sign

## Planned

- Other types, at least integers (8-32bit + signed `int`s)
- Simple optimizations on assembly
  - Setting `RAM_P` multiple times before accessing RAM
  - No bus-reads -> nop
  - Only set upper instruction pointer byte if neccessary (Check location of jump instruction)
  - Check if register is already set to literal values

## _(notes for version two)_

- Simpler tokenization, seperate from parsing
  - Only keywords, names and symbols
  - ExpressionToken is useless
  - Use `struct` for Token types
    - I could write it in C...
- Proper error messages (show problematic line/keyword)
- Proper functions
  - Heap, stack
