#include "cpu.h"

#include <iostream>
/*
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
*/

void printByte(uint8_t input) {
    std::bitset<8> x(input);
    std::cout << x << std::endl;
}

addressingModeize getSizeFromAddressingMode(addressingMode mode) {
    switch (mode) {
        case addressingMode::Implied:
            return addressingModeize::NoBytes;

        case addressingMode::Immediate:
            return addressingModeize::SingleByte;
        case addressingMode::ZeroPage:
            return addressingModeize::SingleByte;
        case addressingMode::XIndexed:
            return addressingModeize::SingleByte;
        case addressingMode::YIndexed:
            return addressingModeize::SingleByte;
        case addressingMode::Registry:
            return addressingModeize::SingleByte;

        case addressingMode::Absolute:
            return addressingModeize::DoubleByte;
        case addressingMode::XIndexedAbsolute:
            return addressingModeize::DoubleByte;
        case addressingMode::YIndexedAbsolute:
            return addressingModeize::DoubleByte;
        default:
            return addressingModeize::SingleByte;
    }
}

uint16_t getOperandData(uint8_t memory[], uint8_t instructionAddress, addressingModeize size) {
    switch (size) {
        case addressingModeize::NoBytes:
            return 0;
        case addressingModeize::SingleByte:
            return memory[instructionAddress + 1];
        case addressingModeize::DoubleByte:
            return (memory[instructionAddress + 1] << 8) + memory[instructionAddress + 2];
        default:
            return 0;
    }
}

CPU::CPU() {
}

CPU::~CPU() {
}

void CPU::loadIntoMemory(uint8_t memory[], int size, uint16_t startAddress) {
    for (size_t i = 0; i < size; i++) {
        RAM[startAddress + i] = memory[i];
    }
}

void CPU::fetch() {
    // PC > MAR
    // RAM[MAR] > MDR

    MAR = PC;
    MDR[0] = (uint16_t)RAM[MAR];
    uint8_t opcode = (uint8_t)(MDR[0] >> 4);
    uint8_t addressMode = (uint8_t)(MDR[0] << 4) >> 4;

    addressingModeize size = getSizeFromAddressingMode((addressingMode)addressMode);

    MDR[1] = getOperandData(RAM, MAR, size);

    PC += (uint16_t)size + 1;
}

int CPU::execute() {
    uint8_t currentOpcode = (uint8_t)(MDR[0] >> 4);
    addressingMode addressMode = (addressingMode)((uint8_t)(MDR[0] << 4) >> 4);
    switch ((opcode)currentOpcode) {
        case opcode::HLT:
            return 0;
        case opcode::ADD:
            if (addressMode == addressingMode::Immediate) {
                ACC += (uint8_t)MDR[1];
            }else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                ACC += RAM[MDR[1]];
            }else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                ACC += RAM[MDR[1] + XRG];
            }else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                ACC += RAM[MDR[1] + YRG];
            }else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                ACC += (*reg);
            }
            break;
        case opcode::SUB:
            if (addressMode == addressingMode::Immediate) {
                ACC -= (uint8_t)MDR[1];
            }else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                ACC -= RAM[MDR[1]];
            }else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                ACC -= RAM[MDR[1] + XRG];
            }else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                ACC -= RAM[MDR[1] + YRG];
            }else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                ACC -= (*reg);
            }
            break;
        case opcode::OUT:
            if (addressMode == addressingMode::Immediate) {
                std::cout << (int)(MDR[1]);
            }else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                std::cout << (int)RAM[MDR[1]];
            }else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                std::cout << (int)RAM[MDR[1] + XRG];
            }else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                std::cout << (int)RAM[MDR[1] + YRG];
            }else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                std::cout << (int)(*reg);
            }
            break;
        case opcode::STA:
            if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                //std::cout << RAM[MDR[1]];
                RAM[MDR[1]] = ACC;
            }else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                RAM[MDR[1] + XRG] = ACC;
            }else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                RAM[MDR[1] + YRG] = ACC;
            }
            break;

        default:
            break;
    }

    return 1;
}

uint8_t* CPU::getRegister(registerAddresses registerAddress) {
    switch (registerAddress)
    {
    case registerAddresses::Accumulator :
        return &ACC;
    case registerAddresses::XRegister :
        return &XRG;
    case registerAddresses::YRegister :
        return &YRG;

    default:
        break;
    }
    return &ACC;
}