#include "../include/Process.h"
//#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <chrono>
#include <cstdlib>

Process::Process(int pid, const std::string &name, int memoryRequired, int diskRequired)
    : pid(pid), name(name), state(NEW), memoryRequired(memoryRequired), diskRequired(diskRequired), turnaroundTime(0)
{
    creationTime = std::chrono::steady_clock::now();
}

Process::~Process()
{
    if (state == RUNNING)
    {
        stopProcess();
    }
}

void Process::ready()
{
    if (state == NEW || state == BLOCKED)
    {
        state = READY;
    }
}

void Process::run()
{
    if (state == READY)
    {
        state = RUNNING;
        startTime = std::chrono::steady_clock::now();
    }
}

void Process::block()
{
    if (state == RUNNING)
    {
        state = BLOCKED;
    }
}

void Process::terminate()
{
    if (state != TERMINATED)
    {
        state = TERMINATED;
        endTime = std::chrono::steady_clock::now();
        stopProcess();
    }
}

bool Process::startProcess()
{
    // Get the current directory for absolute paths
    char currentDir[1024];
    if (getcwd(currentDir, sizeof(currentDir)) == NULL)
    {
        return false;
    }
    std::string absPath = std::string(currentDir) + "/build/" + name;

    // Check if the executable exists before trying to start it
    std::string checkCmd = "if [ -f " + absPath + " ]; then echo 'Exists'; else echo 'Missing'; fi";
    std::string result = "";
    char buffer[128];
    FILE *pipe = popen(checkCmd.c_str(), "r");
    if (!pipe)
    {
        return false;
    }
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);

    // Special handling for calculator to ensure it's properly tracked
    if (name == "calculator")
    {
        // Use exec to launch the process in a new terminal with absolute path
        std::string command = "osascript -e 'tell app \"Terminal\" to do script \"" +
                              absPath + " " +
                              std::to_string(pid) + " " + std::to_string(memoryRequired) + " " +
                              std::to_string(diskRequired) + "\"' >/dev/null 2>&1";

        int cmdResult = std::system(command.c_str());

        if (cmdResult == 0)
        {
            state = RUNNING;
            return true;
        }

        return false;
    }

    // Use exec to launch the process in a new terminal with absolute path
    std::string command = "osascript -e 'tell app \"Terminal\" to do script \"" +
                          absPath + " " +
                          std::to_string(pid) + " " + std::to_string(memoryRequired) + " " +
                          std::to_string(diskRequired) + "\"' >/dev/null 2>&1";

    int cmdResult = std::system(command.c_str());

    if (cmdResult == 0)
    {
        state = RUNNING;
        return true;
    }

    return false;
}

bool Process::stopProcess()
{
    // Kill the process with the corresponding PID
    // In a real implementation, we'd use kill()
    // For simulation, we'll assume the process terminates itself
    if (state == RUNNING)
    {
        state = TERMINATED;
        return true;
    }
    return false;
}

bool Process::isRunning() const
{
    return state == RUNNING;
}

void Process::setTurnaroundTime(int seconds)
{
    turnaroundTime = seconds;
}

int Process::calculateExecutionTime() const
{
    if (state == TERMINATED && startTime != std::chrono::steady_clock::time_point())
    {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
        return static_cast<int>(duration);
    }
    if (state == RUNNING && startTime != std::chrono::steady_clock::time_point())
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        return static_cast<int>(duration);
    }
    return 0;
}
