#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>
#include <thread>

using namespace std;

enum ProcessState
{
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

class Process
{
private:
    int pid;
    string name;
    ProcessState state;
    int memoryRequired;
    int diskRequired;
    int turnaroundTime; // In seconds
    chrono::steady_clock::time_point creationTime;
    chrono::steady_clock::time_point startTime;
    chrono::steady_clock::time_point endTime;
    thread taskThread;

public:
    Process(int pid, const string &name, int memoryRequired, int diskRequired);
    ~Process();

    // Getters
    int getPid() const { return pid; }
    string getName() const { return name; }
    ProcessState getState() const { return state; }
    int getMemoryRequired() const { return memoryRequired; }
    int getDiskRequired() const { return diskRequired; }
    int getTurnaroundTime() const { return turnaroundTime; }

    // State transitions
    void ready();
    void run();
    void block();
    void terminate();

    // Process execution
    bool startProcess();
    bool stopProcess();
    bool isRunning() const;

    // Resource management
    void setTurnaroundTime(int seconds);
    int calculateExecutionTime() const;
};

#endif // PROCESS_H
