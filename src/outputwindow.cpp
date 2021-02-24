#include "outputwindow.h"


OutputWindow::OutputWindow() {
}

OutputWindow::~OutputWindow() {

}

void OutputWindow::init() {
    app = Gtk::Application::create("uk.gsck.lmc");

    auto t1 = Gtk::TextBuffer::create();
    auto t2 = Gtk::TextBuffer::create();
    instructionBuffer = &t1;
    outputBuffer = &t2;

    outputText = new Gtk::TextView;
    instructionText = new Gtk::TextView;

    outputText->set_buffer(*outputBuffer);
    instructionText->set_buffer(*instructionBuffer);

    Gtk::Window newWindow;
    window = &newWindow;

    window->set_default_size(800, 300);
    window->set_title("Output");

    Gtk::Box box;
    box.set_margin_start(10);
    box.set_margin_end(10);
    box.set_margin_top(10);
    box.set_margin_bottom(10);

    Gtk::Frame outputFrame;
    outputText->set_editable(false);
    outputFrame.add(*outputText);
    outputFrame.set_label("Output");

    Gtk::Frame instructionFrame;
    instructionText->set_editable(false);
    instructionFrame.add(*instructionText);
    instructionFrame.set_label("Instructions");

    Gtk::ScrolledWindow instructionsScrolledWindow;
    instructionsScrolledWindow.add(instructionFrame);

    Gtk::ScrolledWindow outputScrolledWindow;
    outputScrolledWindow.add(outputFrame);

    box.pack_start(outputScrolledWindow);
    box.pack_end(instructionsScrolledWindow);

    box.show_all();

    window->add(box);

    /*
        Init CPU thread
        TODO make reactive to button press
     */
    cpuThread = new std::thread([this] {
        cpuWorker.startExecution(this);
    });

    app->run(*window);
}

void OutputWindow::print(MessageType type, std::string string) {
    //std::cout << string;
    if (type == MessageType::Output) {
        auto iter = outputBuffer->get()->end();
        outputBuffer->get()->insert(iter, string);
    } else {
        auto iter = instructionBuffer->get()->end();
        instructionBuffer->get()->insert(iter, string);
    }
}