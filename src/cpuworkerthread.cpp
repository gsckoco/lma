#include "cpuworkerthread.h"
#include "cpu.h"

cpuWorkerThread::cpuWorkerThread() {

}

void cpuWorkerThread::startExecution(OutputWindow *caller) {
    //cpu->loadIntoMemory(memory, 0xFFFF, 0x0000);

    CPU *cpu = new CPU(caller);

    uint8_t program[] = {
        0x40, 0x0A, // LDA 10
        0x88, 0x00, // OUT A
        0x90, 0x0A, // OTC 0x0A newline
        0x20, 0x01, // SUB 1
        0x73, 0x02, // BRP 0x02
        0x80, 0x7F  // OUT 0x75
    };
    cpu->loadIntoMemory(program, sizeof(program), 0x0000);
    //cpu->loadIntoMemory(program2, sizeof(program2), 0x00FF);
    cpu->start();
}