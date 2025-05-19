#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
//#include <vector>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

bool running = true;
std::string content;
int pid = 0;
std::string filename;

// Signal handler for graceful shutdown
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "Notepad shutting down..." << std::endl;
        running = false;
    }
}

// Function to autosave content to file
void autoSave()
{
    while (running)
    {
        if (!content.empty())
        {
            std::ofstream outFile("simulated_disk/" + filename);
            if (outFile.is_open())
            {
                outFile << content;
                outFile.close();
                std::cout << "Auto-saved content to " << filename << std::endl;
            }
            else
            {
                std::cerr << "Failed to open file for autosave" << std::endl;
            }
        }

        // Sleep for 10 seconds before next autosave
        std::this_thread::sleep_for(std::chrono::seconds(10));

        // Exit if not running
        if (!running)
            break;
    }
}

void displayMenu()
{
    std::cout << "\n===== Notepad Menu =====\n";
    std::cout << "1. View content\n";
    std::cout << "2. Add text\n";
    std::cout << "3. Clear content\n";
    std::cout << "4. Save and exit\n";
    std::cout << "Enter choice: ";
}

int main(int argc, char *argv[])
{
    // Check command line arguments (pid, memory, disk)
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <pid> <memory_required> <disk_required>" << std::endl;
        return 1;
    }

    pid = std::stoi(argv[1]);
    int memoryRequired = std::stoi(argv[2]);
    int diskRequired = std::stoi(argv[3]);

    // Set up the filename
    filename = "note_" + std::to_string(pid) + ".txt";

    // Print task information
    std::cout << "Starting Notepad (PID: " << pid << ")" << std::endl;
    std::cout << "Memory required: " << memoryRequired << " MB" << std::endl;
    std::cout << "Disk required: " << diskRequired << " MB" << std::endl;

    // Register signal handler
    signal(SIGINT, signalHandler);

    // Create directory if it doesn't exist
    system("mkdir -p simulated_disk");

    // Start autosave thread
    std::thread autosaveThread(autoSave);
    autosaveThread.detach();

    // Main loop
    int choice;
    std::string input;

    while (running)
    {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

        switch (choice)
        {
        case 1: // View content
            std::cout << "\n--- Document Content ---" << std::endl;
            if (content.empty())
            {
                std::cout << "[Empty document]" << std::endl;
            }
            else
            {
                std::cout << content << std::endl;
            }
            break;

        case 2: // Add text
            std::cout << "Enter text (type ':q' on a new line to finish):" << std::endl;
            while (true)
            {
                std::string line;
                std::getline(std::cin, line);
                if (line == ":q")
                    break;
                content += line + "\n";
            }
            break;

        case 3: // Clear content
            content.clear();
            std::cout << "Content cleared" << std::endl;
            break;

        case 4: // Save and exit
            running = false;
            break;

        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    // Final save before exit
    std::ofstream outFile("simulated_disk/" + filename);
    if (outFile.is_open())
    {
        outFile << content;
        outFile.close();
        std::cout << "Final save to " << filename << std::endl;
    }

    std::cout << "Notepad exiting..." << std::endl;
    return 0;
}
