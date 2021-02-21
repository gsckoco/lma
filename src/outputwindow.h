#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <gtkmm.h>
#include <string>
#include <thread>
#include "cpu.h"
#include "cpuworkerthread.h"

enum class MessageType {
    Instruction,
    Output
};
// Forward declaration
class cpuWorkerThread;

class OutputWindow {
   private:
    /* data */
    Glib::RefPtr<Gtk::Application> app;
    Gtk::Window *window = nullptr;
    std::thread *cpuThread = nullptr;
    cpuWorkerThread cpuWorker;
   public:
    Gtk::TextView *outputText = nullptr;
    Gtk::TextView *instructionText = nullptr;
    Glib::RefPtr<Gtk::TextBuffer> *instructionBuffer, *outputBuffer = nullptr;
    OutputWindow();
    ~OutputWindow();
    void init();
    void print(MessageType type, std::string string);
};

#endif // OUTPUTWINDOW_H
