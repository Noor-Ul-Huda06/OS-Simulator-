#include "../include/OSSystem.h"
#include <iostream>
#include <thread>
#include <string>
#include <csignal>
#include <cstdlib>

OSSystem os;
bool running = true;

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "\nInterrupt received, shutting down...\n";
        running = false;
        os.shutdownSystem();
        exit(0);
    }
}

void displayMenu()
{
    // Show resource status at the top
    os.showResourceStatus();

    std::cout << "\n===== OS Simulator Menu =====\n";
    std::cout << "1. Launch a task\n";
    std::cout << "2. Show system status\n";
    std::cout << "3. List tasks\n";
    std::cout << "4. Close a task\n";
    std::cout << "5. Minimize a task\n";
    std::cout << "6. Resume a task\n";
    std::cout << "7. List running tasks\n";
    std::cout << "8. Shutdown\n";
    std::cout << "m. Switch mode (User/Kernel)\n";

    // Display current mode in the prompt
    if (os.getCurrentMode() == USER_MODE)
    {
        std::cout << "[User]> ";
    }
    else
    {
        std::cout << "[Kernel]> ";
    }
}

int main(int argc, char *argv[])
{
    // Register signal handler for ctrl+c
    std::signal(SIGINT, signalHandler);

    // Check for command-line arguments
    if (argc == 4)
    {
        int ram = std::atoi(argv[1]);   // RAM in MB
        int disk = std::atoi(argv[2]);  // Disk in GB (convert to MB)
        int cores = std::atoi(argv[3]); // Number of cores

        // Initialize with command-line arguments
        os.initialize(ram, disk * 1024, cores); // Convert disk from GB to MB
    }
    else
    {
        // Initialize with interactive prompts - without requiring initial Enter
        os.initialize();
    }

    os.bootSystem();

    // Start scheduler thread
    std::thread schedulerThread(&OSSystem::scheduler, &os);
    schedulerThread.detach();

    // Launch automatic background services
    os.createProcess("clock", 15, 2);
    os.createProcess("calendar", 25, 8);

    std::string choice;
    while (running && os.isSystemRunning())
    {
        displayMenu();
        std::cin >> choice;

        if (choice == "m" || choice == "M")
        {
            // Toggle between user and kernel mode
            if (os.getCurrentMode() == USER_MODE)
            {
                os.switchToKernelMode();
            }
            else
            {
                os.switchToUserMode();
            }
            continue;
        }

        // Convert choice to integer for numeric options
        int numChoice;
        try
        {
            numChoice = std::stoi(choice);
        }
        catch (...)
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }

        switch (numChoice)
        {
        case 1:
        { // Launch a task
            os.showAvailableTasks();
            int taskNum;
            std::cout << "Enter task number to launch: ";
            std::cin >> taskNum;

            std::string taskName;
            int ramRequired = 0, diskRequired = 0;

            switch (taskNum)
            {
            case 1:
                taskName = "notepad";
                ramRequired = 50;
                diskRequired = 10;
                break;
            case 2:
                taskName = "calculator";
                ramRequired = 20;
                diskRequired = 5;
                break;
            case 3:
                taskName = "clock";
                ramRequired = 15;
                diskRequired = 2;
                break;
            case 4:
                taskName = "file_manager";
                ramRequired = 40;
                diskRequired = 5;
                break;
            case 5:
                taskName = "minesweeper";
                ramRequired = 30;
                diskRequired = 10;
                break;
            case 6:
                taskName = "music_player";
                ramRequired = 60;
                diskRequired = 20;
                break;
            case 7:
                taskName = "calendar";
                ramRequired = 25;
                diskRequired = 8;
                break;
            case 8:
                taskName = "timer";
                ramRequired = 10;
                diskRequired = 2;
                break;
            case 9:
                taskName = "terminal";
                ramRequired = 15;
                diskRequired = 3;
                break;
            case 10:
                taskName = "system_monitor";
                ramRequired = 25;
                diskRequired = 4;
                break;
            case 11:
                taskName = "settings";
                ramRequired = 20;
                diskRequired = 3;
                break;
            default:
                std::cout << "Invalid task number" << std::endl;
                continue;
            }

            int pid = os.createProcess(taskName, ramRequired, diskRequired);
            if (pid != -1)
            {
                std::cout << "Task launched with PID: " << pid << std::endl;
            }
            break;
        }
        case 2: // Show system status
            os.showSystemStatus();
            break;

        case 3: // List tasks
            os.showAvailableTasks();
            break;

        case 4:
        { // Close a task
            int pid;
            std::cout << "Enter PID of task to close: ";
            std::cin >> pid;

            if (os.getCurrentMode() == USER_MODE)
            {
                // In user mode, can only close processes that were launched explicitly
                bool success = os.terminateProcess(pid);
                if (success)
                {
                    std::cout << "Task terminated successfully" << std::endl;
                }
                else
                {
                    std::cout << "Failed to terminate task: Invalid PID or insufficient permissions" << std::endl;
                }
            }
            else
            {
                // In kernel mode, can close any process
                bool success = os.terminateProcess(pid);
                if (success)
                {
                    std::cout << "Task terminated successfully (Kernel Mode)" << std::endl;
                }
                else
                {
                    std::cout << "Failed to terminate task: Invalid PID" << std::endl;
                }
            }
            break;
        }

        case 5:
        { // Minimize a task
            int pid;
            cout << "Enter PID of task to minimize: ";
            std::cin >> pid;

            bool success = os.minimizeProcess(pid);
            if (success)
            {
            cout << "Task minimized" << endl;
            }
            else
            {
                std::cout << "Failed to minimize task: Invalid PID" << std::endl;
            }
            break;
        }

        case 6:
        { // Resume a task
            int pid;
            std::cout << "Enter PID of task to resume: ";
            std::cin >> pid;

            bool success = os.resumeProcess(pid);
            if (success)
            {
                std::cout << "Task resumed" << std::endl;
            }
            else
            {
                std::cout << "Failed to resume task: Invalid PID" << std::endl;
            }
            break;
        }

        case 7: // List running tasks
            os.showRunningTasks();
            break;

        case 8: // Shutdown
            running = false;
            os.shutdownSystem();
            break;

        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    return 0;
}
