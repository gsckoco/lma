#ifndef CPU_H
#define CPU_H
#include <iostream>
#include <bitset>
#include "outputwindow.h"

// Forward declaration
class OutputWindow;

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
    OutputWindow *caller;
   public:
    CPU(OutputWindow *caller);
    ~CPU();
    void InstructionPrint(std::string input);
    void OutputPrint(std::string input);
    void loadIntoMemory(uint8_t memory[], int size, uint16_t startAddress);
    void fetch();
    int execute();
    uint8_t* getRegister(registerAddresses registerAddress);
    int start();
};


#endif // !CPU_H