#include "../include/Scheduler.h"
#include "../include/ResourceManager.h"
#include <iostream>
#include <algorithm>

using namespace std;

Scheduler::Scheduler(ResourceManager &resourceManager)
    : resourceManager(resourceManager), currentTime(0)
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::addProcess(Process *process)
{
    readyQueue.push_back(process);
}

void Scheduler::schedule()
{
    if (readyQueue.empty())
    {
        return;
    }

    // Sort processes by priority (FCFS for now)
    sort(readyQueue.begin(), readyQueue.end(),
         [](const Process *a, const Process *b)
         {
             return a->getPid() < b->getPid();
         });

    // Get the next process to run
    Process *nextProcess = readyQueue.front();
    readyQueue.erase(readyQueue.begin());

    // Allocate resources
    if (resourceManager.allocateResources(nextProcess->getMemoryRequired(),
                                        nextProcess->getDiskRequired(), 1))
    {
        nextProcess->startProcess();
        runningProcesses.push_back(nextProcess);
    }
    else
    {
        // If resources aren't available, put the process back in the queue
        readyQueue.push_back(nextProcess);
    }
}

void Scheduler::update()
{
    currentTime++;

    // Update running processes
    for (auto it = runningProcesses.begin(); it != runningProcesses.end();)
    {
        Process *process = *it;
        if (process->isRunning())
        {
            // Simulate process execution
            if (process->calculateExecutionTime() >= 5)
            {
                process->terminate();
                resourceManager.releaseResources(process->getMemoryRequired(),
                                               process->getDiskRequired(), 1);
                it = runningProcesses.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
}

void Scheduler::printStatus() const
{
    cout << "\n=== Scheduler Status ===" << endl;
    cout << "Current Time: " << currentTime << endl;
    cout << "Ready Queue Size: " << readyQueue.size() << endl;
    cout << "Running Processes: " << runningProcesses.size() << endl;
    cout << "=====================" << endl;
}
