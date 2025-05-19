#ifndef OS_SYSTEM_H
#define OS_SYSTEM_H

#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>
#include "Process.h"

using namespace std;

enum SystemMode
{
    USER_MODE,
    KERNEL_MODE
};

class OSSystem
{
private:
    // System resources
    int totalRam;
    int availableRam;
    int totalDisk;
    int availableDisk;
    int totalCores;
    int availableCores;

    // Process management
    vector<shared_ptr<Process>> runningProcesses;
    vector<shared_ptr<Process>> blockedProcesses;
    queue<shared_ptr<Process>> readyQueue;

    // Synchronization
    mutex resourceMutex;
    mutex queueMutex;
    condition_variable schedulerCV;

    // System state
    SystemMode currentMode;
    bool isRunning;

    // Process ID counter
    int nextPid;

    // File system simulation
    unordered_map<string, int> fileSystem; // filename -> size

public:
    OSSystem();

    // System initialization
    void initialize();                             // Interactive initialization
    void initialize(int ram, int disk, int cores); // Command-line initialization
    void bootSystem();

    // Resource management
    bool allocateResources(int ramRequired, int diskRequired);
    void freeResources(int ramToFree, int diskToFree);

    // Process management
    int createProcess(const string &processName, int ramRequired, int diskRequired);
    bool terminateProcess(int pid);
    bool minimizeProcess(int pid);
    bool resumeProcess(int pid);

    // Scheduling
    void scheduler();
    void dispatchProcesses();

    // Mode switching
    void switchToUserMode();
    void switchToKernelMode();

    // File system operations
    bool createFile(const string &filename, int size);
    bool deleteFile(const string &filename);

    // System operations
    void showSystemStatus();
    void showResourceStatus();
    void showAvailableTasks();
    void showRunningTasks();
    void syncRunningProcesses();
    void shutdownSystem();

    // Getters
    bool isSystemRunning() const { return isRunning; }
    SystemMode getCurrentMode() const { return currentMode; }
    int getUsedCores() const { return runningProcesses.size(); }
    int getTotalCores() const { return totalCores; }
};

#endif // OS_SYSTEM_H
