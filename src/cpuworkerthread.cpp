#include "cpuworkerthread.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "cpu.h"

cpuWorkerThread::cpuWorkerThread() {
}

std::vector<uint8_t> readFile(std::ifstream *file) {
    // Stop eating new lines in binary mode!!!
    file->unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file->seekg(0, std::ios::end);
    fileSize = file->tellg();
    file->seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(), std::istream_iterator<uint8_t>(*file), std::istream_iterator<uint8_t>());

    return vec;
}

void cpuWorkerThread::startExecution(OutputWindow *caller) {
    //cpu->loadIntoMemory(memory, 0xFFFF, 0x0000);

    CPU *cpu = new CPU(caller);

    std::vector<uint8_t> program = readFile(caller->program);

    cpu->loadIntoMemory(program, 0x0000);
    //cpu->loadIntoMemory(program2, sizeof(program2), 0x00FF);
    cpu->start();
}