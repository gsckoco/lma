#include "cpu.h"

#include <fmt/core.h>
#include <gtkmm.h>

#include <iomanip>
#include <iostream>
#include <string>

void printByte(uint8_t input) {
    std::bitset<8> x(input);
    std::cout << x << std::endl;
}

addressingModeSize getSizeFromAddressingMode(addressingMode mode) {
    switch (mode) {
        case addressingMode::Implied:
            return addressingModeSize::NoBytes;
        default:
        case addressingMode::Immediate:
        case addressingMode::ZeroPage:
        case addressingMode::XIndexed:
        case addressingMode::YIndexed:
        case addressingMode::Registry:
            return addressingModeSize::SingleByte;

        case addressingMode::Absolute:
        case addressingMode::XIndexedAbsolute:
        case addressingMode::YIndexedAbsolute:
            return addressingModeSize::DoubleByte;
    }
}

uint16_t getOperandData(uint8_t memory[], uint8_t instructionAddress, addressingModeSize size) {
    switch (size) {
        case addressingModeSize::NoBytes:
            return 0;
        case addressingModeSize::SingleByte:
            return memory[instructionAddress + 1];
        case addressingModeSize::DoubleByte:
            return (memory[instructionAddress + 1] << 8) + memory[instructionAddress + 2];
        default:
            return 0;
    }
}

CPU::CPU(OutputWindow *caller) {
    this->caller = caller;
}

CPU::~CPU() {
}

void CPU::InstructionPrint(std::string input) {
    caller->print(MessageType::Instruction, input);
}

void CPU::OutputPrint(std::string input) {
    caller->print(MessageType::Output, input);
}

void CPU::loadIntoMemory(std::vector<uint8_t> input, uint16_t startAddress) {
    uint16_t pointer = 0;
    for (uint8_t byte : input) {
        RAM[pointer + startAddress] = byte;
        pointer++;
    }
}

void CPU::fetch() {
    // PC > MAR
    // RAM[MAR] > MDR

    MAR = PC;
    MDR[0] = (uint16_t)RAM[MAR];
    uint8_t opcode = (uint8_t)(MDR[0] >> 4);
    uint8_t addressMode = (uint8_t)(MDR[0] << 4) >> 4;

    addressingModeSize size = getSizeFromAddressingMode((addressingMode)addressMode);

    MDR[1] = getOperandData(RAM, MAR, size);
    PC += (uint16_t)size + 1;
}

std::string instructionName(opcode instruction) {
    switch (instruction) {
        case opcode::HLT:
            return "HLT";
        case opcode::ADD:
            return "ADD";
        case opcode::SUB:
            return "SUB";
        case opcode::STA:
            return "STA";
        case opcode::LDA:
            return "LDA";
        case opcode::BRA:
            return "BRA";
        case opcode::BRZ:
            return "BRZ";
        case opcode::BRP:
            return "BRP";
        case opcode::OUT:
            return "OUT";
        case opcode::OTC:
            return "OTC";
        case opcode::LDX:
            return "LDX";
        case opcode::LDY:
            return "LDY";
        case opcode::INX:
            return "INX";
        case opcode::INY:
            return "INY";

        default:
            break;
    }
    return "";
}

void printHex(int number) {
    std::cout << std::hex << std::uppercase << number << std::dec << std::nouppercase;
}

int CPU::execute() {
    uint8_t currentOpcode = (uint8_t)(MDR[0] >> 4);
    addressingMode addressMode = (addressingMode)((uint8_t)(MDR[0] << 4) >> 4);
    InstructionPrint(fmt::format("{} 0x{:04X} @ 0x{:04X}\n", instructionName((opcode)currentOpcode), MDR[1], MAR));

    switch ((opcode)currentOpcode) {
        case opcode::HLT:
            return 0;
        case opcode::ADD:
            if (addressMode == addressingMode::Immediate) {
                ACC += (uint8_t)MDR[1];
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                ACC += RAM[MDR[1]];
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                ACC += RAM[MDR[1] + XRG];
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                ACC += RAM[MDR[1] + YRG];
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                ACC += (*reg);
            }
            break;
        case opcode::SUB:
            if (addressMode == addressingMode::Immediate) {
                ACC -= (uint8_t)MDR[1];
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                ACC -= RAM[MDR[1]];
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                ACC -= RAM[MDR[1] + XRG];
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                ACC -= RAM[MDR[1] + YRG];
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                ACC -= (*reg);
            }
            break;
        case opcode::STA:
            if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                RAM[MDR[1]] = ACC;
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                RAM[MDR[1] + XRG] = ACC;
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                RAM[MDR[1] + YRG] = ACC;
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                *reg = ACC;
            }
            break;
        case opcode::LDA:
            if (addressMode == addressingMode::Immediate) {
                ACC = MDR[1];
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                ACC = RAM[MDR[1]];
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                ACC = RAM[MDR[1] + XRG];
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                ACC = RAM[MDR[1] + YRG];
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                ACC = *reg;
            }
            break;
        case opcode::BRA:  // Goto address
            if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                PC = MDR[1];
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                PC = MDR[1] + XRG;
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                PC = MDR[1] + YRG;
            }
            break;
        case opcode::BRZ:  // Goto address when accumulator == 0
            if (ACC == 0) {
                if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                    PC = MDR[1];
                } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                    PC = MDR[1] + XRG;
                } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                    PC = MDR[1] + YRG;
                }
            }
            break;
        case opcode::BRP:  // Goto address when accumulator > 0
            if (ACC > 0) {
                if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                    PC = MDR[1];
                } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                    PC = MDR[1] + XRG;
                } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                    PC = MDR[1] + YRG;
                }
            }
            break;
        case opcode::OUT:
            if (addressMode == addressingMode::Immediate) {
                OutputPrint(fmt::format("{}", (int)(MDR[1])));
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                OutputPrint(fmt::format("{}", (int)RAM[MDR[1]]));
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                OutputPrint(fmt::format("{}", (int)RAM[MDR[1] + XRG]));
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                OutputPrint(fmt::format("{}", (int)RAM[MDR[1] + YRG]));
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                OutputPrint(fmt::format("{}", (int)(*reg)));
            }
            break;
        case opcode::OTC:
            if (addressMode == addressingMode::Immediate) {
                OutputPrint(fmt::format("{}", (char)(MDR[1])));
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                OutputPrint(fmt::format("{}", (char)RAM[MDR[1]]));
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                OutputPrint(fmt::format("{}", (char)RAM[MDR[1] + XRG]));
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                OutputPrint(fmt::format("{}", (char)RAM[MDR[1] + YRG]));
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                OutputPrint(fmt::format("{}", (char)(*reg)));
            }
            break;
        case opcode::LDX:
            if (addressMode == addressingMode::Immediate) {
                XRG = MDR[1];
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                XRG = RAM[MDR[1]];
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                XRG = RAM[MDR[1] + XRG];
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                XRG = RAM[MDR[1] + YRG];
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                XRG = *reg;
            }
            break;
        case opcode::LDY:
            if (addressMode == addressingMode::Immediate) {
                YRG = MDR[1];
            } else if (addressMode == addressingMode::Absolute || addressMode == addressingMode::ZeroPage) {
                YRG = RAM[MDR[1]];
            } else if (addressMode == addressingMode::XIndexed || addressMode == addressingMode::XIndexedAbsolute) {
                YRG = RAM[MDR[1] + XRG];
            } else if (addressMode == addressingMode::YIndexed || addressMode == addressingMode::YIndexedAbsolute) {
                YRG = RAM[MDR[1] + YRG];
            } else if (addressMode == addressingMode::Registry) {
                uint8_t *reg = getRegister((registerAddresses)MDR[1]);
                YRG = *reg;
            }
            break;
        case opcode::INX:
            XRG += 1;
            break;
        case opcode::INY:
            YRG += 1;
            break;

        default:
            return 0;
    }
    return 1;
}

void CPU::step() {
    fetch();
    execute();
}

int CPU::start() {
    int lastStatus = 1;
    while (lastStatus != 0) {
        fetch();
        lastStatus = execute();
    }
    return lastStatus;
}

uint8_t *CPU::getRegister(registerAddresses registerAddress) {
    switch (registerAddress) {
        case registerAddresses::Accumulator:
            return &ACC;
        case registerAddresses::XRegister:
            return &XRG;
        case registerAddresses::YRegister:
            return &YRG;

        default:
            break;
    }
    return &ACC;
}