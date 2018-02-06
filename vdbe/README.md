# independent vdbe

## instructions.sh

Parse string output of explain from sql console to comma separated opcode instruction list

## opcodes.sh

Parse sqlite header file opcodes.h to generate a map of opcode strings used in explain to opcode numbers (based on index in array)

## sqlite

Build with: 

`CFLAGS="-gdwarf-4 -O0" ../configure --disable-tcl --disable-amalgamation --disable-threadsafe`
