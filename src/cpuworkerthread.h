#ifndef CPUWORKERTHREAD_H
#define CPUWORKERTHREAD_H

// Forward declaration
class OutputWindow;

class cpuWorkerThread {
   private:
   public:
    cpuWorkerThread();
    void startExecution(OutputWindow *caller);
};

#endif // !CPUWORKERTHREAD_H