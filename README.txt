HLT   0x0X -- Halts the processor, ending execution
ADD x 0x1X -- Adds x to whatever is currently in the accumulator.
SUB x 0x2X -- Subtracts x to whatever is currently in the accumulator.
STA x 0x3X -- Moves accumulator into memory location x
LDA x 0x4X -- Moves value from memory location x into accumulator
BRA x 0x5X -- Moves program counter to memory location x
BRZ x 0x6X -- Moves program counter to memory location x, if accumulator value is 0
BRP x 0x7X -- Moves program counter to memory location x, if accumulator value is greater than 0
OUT x 0x8X -- Outputs the value of x
OTC x 0x9X -- Outputs the value of x as a character

Register Aliases:
A -- Accumulator

Addressing modes:
Immediate        0xX0 -- Takes value as operand, 1 byte
Implied          0xX1 -- Doesn't take in any value, 0 bytes
Absolute         0xX2 -- Takes absolute memory address, 2 bytes
ZeroPage         0xX3 -- Only capable of addressing first 256 bytes in memory, 1 byte
XIndexed         0xX4 -- Adds address of X register to a zeropage address, 1 byte
YIndexed         0xX5 -- Adds address of Y register to a zeropage address, 1 byte
XIndexedAbsolute 0xX6 -- Adds address of X register to an absolute address, 2 bytes
YIndexedAbsolute 0xX7 -- Adds address of Y register to an absolute address, 2 bytes
Registry         0xX8 -- Points to value of registry, 1 bytes

Big Endian

Proper README will come later