#include "main.h"

#include <bitset>
#include <iostream>

int main() {
    CPU *cpu = new CPU();
    uint8_t program[] = {
        0x10, 0x0A, 0x20, 0x05, // ADD 10, SUB 5
        0x32, 0x00, 0xFF,       // STA 0x00FF
        0x88, 0x00, 0x00,       // OUT Accumulator
    };
    cpu->loadIntoMemory(program, sizeof(program), 0x00);
    int lastStatus = 1;
    while (lastStatus != 0) {
        cpu->fetch();
        lastStatus = cpu->execute();
    }
    std::cout << std::endl;
}