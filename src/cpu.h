#include <iostream>
#include <bitset>

/*

ADD x 0x1X -- Adds x to whatever is currently in the accumulator.
SUB x 0x2X -- Subtracts x to whatever is currently in the accumulator.
STA x 0x3X -- Moves accumulator into memory location x
LDA x 0x4X -- Moves value from memory location x into accumulator
BRA x 0x5X -- Moves program counter to memory location x
BRZ x 0x6X -- Moves program counter to memory location x, if accumulator value is 0
BRP x 0x7X -- Moves program counter to memory location x, if accumulator value is greater than 0
OUT x 0x8X -- Outputs the value of x
HLT   0x0X -- Halts the processor, ending execution

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

*/

enum class opcode : uint8_t {
    HLT = 0x00,
    ADD = 0x01,
    SUB = 0x02,
    STA = 0x03,
    LDA = 0x04,
    BRA = 0x05,
    BRZ = 0x06,
    BRP = 0x07,
    OUT = 0x08,
    OTC = 0x09
};

enum class addressingMode : uint8_t {
    Immediate = 0x00,
    Implied = 0x01,
    Absolute = 0x02,
    ZeroPage = 0x03,
    XIndexed = 0x04,
    YIndexed = 0x05,
    XIndexedAbsolute = 0x06,
    YIndexedAbsolute = 0x07,
    Registry = 0x08
};

enum class addressingModeize : uint8_t {
    NoBytes,
    SingleByte,
    DoubleByte
};

enum class registerAddresses : uint8_t {
    Accumulator = 0x00,
    XRegister,
    YRegister
};

void printByte(uint8_t input);
addressingModeize getSizeFromAddressingMode(addressingMode mode);
uint16_t getOperandData(uint8_t memory[], uint8_t instructionAddress, addressingModeize size);

class CPU {
   private:
    uint16_t PC = 0;
    uint16_t MDR[2] = {0, 0};  // Instruction, Data
    uint8_t RAM[0xFFFF];
    uint16_t MAR = 0;
    uint8_t CIR = 0;
    uint8_t ACC = 0;
    uint8_t XRG = 0; // X Register
    uint8_t YRG = 0; // Y Register
   public:
    CPU();
    ~CPU();
    void loadIntoMemory(uint8_t memory[], int size, uint16_t startAddress);
    void fetch();
    int execute();
    uint8_t* getRegister(registerAddresses registerAddress);
};