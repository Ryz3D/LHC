# The _Large Hadron Compiler_

It's not really large, this project generates assembly and machine code for a little computer with a very limited instruction set. As a source it targets a subset of C.
I fully intend to build a machine capable of executing the generated binaries in hardware, but time will tell if I'm actually able to.

## Implemented

_(pretty much)_

- Parsing .c files
- Basic variables (`uint_8`, `bool`)
  - All variables are treated as global for now
- Custom CPU simulator (and brainfuck, just for fun)
- Output and parse custom assembly
  - Can be used as an assembly debugger/simulator and assembly->binary converter
- Output as binary (for EEPROM and LogiSim)

## TO-DO

- Memory management (allocating and freeing RAM)
  - `free` local variables after scope ended
- Support `if` by conditional jumps
- Handle `else` by caching result of `if` condition

## Planned

- Actual loops (`while`, `for`)
- At least other integer types (8-32bit + signed `int`s)
- Support as many logical operators as possible
