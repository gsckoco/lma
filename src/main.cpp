#include "main.h"

#include <bitset>
#include <iostream>
#include <thread>

//void guiThread(OutputWindow *window) {
//    window->init();
//}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        // TODO display arguments on error
        std::cerr << "Not enough arguments." << std::endl;
        return 0;
    }
    std::string inputFile(argv[1]);
    std::cout << "Reading from file: " << inputFile << std::endl;
    OutputWindow window;
    window.program = new std::ifstream(inputFile, std::ios::binary);
    window.init();
}