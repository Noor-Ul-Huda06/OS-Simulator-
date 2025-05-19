#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Process.h"

using namespace std;

// Forward declaration
class ResourceManager;

enum SchedulerType
{
    FCFS,     // First Come First Serve
    PRIORITY, // Priority-based
    RR        // Round Robin
};

// Structure for multilevel queue
struct MultilevelQueue
{
    queue<shared_ptr<Process>> highPriorityQueue;   // Priority scheduling
    queue<shared_ptr<Process>> mediumPriorityQueue; // Round Robin
    queue<shared_ptr<Process>> lowPriorityQueue;    // FCFS

    mutable mutex queueMutex;
    condition_variable queueCV;
};

class Scheduler
{
private:
    ResourceManager& resourceManager;
    int currentTime;
    vector<Process*> readyQueue;
    vector<Process*> runningProcesses;

public:
    Scheduler(ResourceManager& resourceManager);
    ~Scheduler();

    void addProcess(Process* process);
    void schedule();
    void update();
    void printStatus() const;
};

#endif // SCHEDULER_H
