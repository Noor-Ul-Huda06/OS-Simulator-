#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>
#include <signal.h>
#include <ctime>

bool running = true;
bool paused = false;
bool showDetailed = false;

// System resources
struct SystemResources
{
    int totalRam;
    int usedRam;
    int totalDisk;
    int usedDisk;
    int cpuCores;
    std::vector<int> cpuUsage; // percentage for each core
    int networkUp;             // KB/s
    int networkDown;           // KB/s
    float temperature;         // Celsius
    int batteryLevel;          // percentage
};

SystemResources resources;

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());

// Signal handler for graceful shutdown
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "System Monitor shutting down..." << std::endl;
        running = false;
    }
}

// Function to update system resources randomly (simulated)
void updateResources()
{
    // RAM usage fluctuation
    std::uniform_int_distribution<int> ramDist(-50, 50);
    resources.usedRam += ramDist(gen);
    resources.usedRam = std::clamp(resources.usedRam, 0, resources.totalRam);

    // Disk usage fluctuation
    std::uniform_int_distribution<int> diskDist(-20, 20);
    resources.usedDisk += diskDist(gen);
    resources.usedDisk = std::clamp(resources.usedDisk, 0, resources.totalDisk);

    // CPU usage for each core
    for (int i = 0; i < resources.cpuCores; i++)
    {
        std::uniform_int_distribution<int> cpuDist(-10, 10);
        resources.cpuUsage[i] += cpuDist(gen);
        resources.cpuUsage[i] = std::clamp(resources.cpuUsage[i], 0, 100);
    }

    // Network usage
    std::uniform_int_distribution<int> netDist(-100, 100);
    resources.networkUp += netDist(gen);
    resources.networkDown += netDist(gen);
    resources.networkUp = std::max(0, resources.networkUp);
    resources.networkDown = std::max(0, resources.networkDown);

    // Temperature
    std::uniform_real_distribution<float> tempDist(-0.5f, 0.5f);
    resources.temperature += tempDist(gen);
    resources.temperature = std::clamp(resources.temperature, 20.0f, 90.0f);

    // Battery
    std::uniform_int_distribution<int> batteryDist(-1, 1);
    resources.batteryLevel += batteryDist(gen);
    resources.batteryLevel = std::clamp(resources.batteryLevel, 0, 100);
}

// Function to display a progress bar
void displayProgressBar(int percentage, int width = 20)
{
    int filled = (percentage * width) / 100;
    std::cout << "[";
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
            std::cout << "■";
        else
            std::cout << " ";
    }
    std::cout << "] " << percentage << "%";
}

// Function to display current time
void displayTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm *localTime = std::localtime(&currentTime);

    std::cout << std::setfill('0')
              << std::setw(2) << localTime->tm_hour << ":"
              << std::setw(2) << localTime->tm_min << ":"
              << std::setw(2) << localTime->tm_sec << " "
              << (1900 + localTime->tm_year) << "-"
              << std::setw(2) << (localTime->tm_mon + 1) << "-"
              << std::setw(2) << localTime->tm_mday;
}

// Function to display system resources
void displayResources()
{
    if (paused)
        return;

    // Clear screen
    std::cout << "\033[2J\033[1;1H";

    // Show title
    std::cout << "======= System Monitor =======   ";
    displayTime();
    std::cout << "\n\n";

    // RAM usage
    int ramPercentage = (resources.usedRam * 100) / resources.totalRam;
    std::cout << "RAM Usage: " << resources.usedRam << " MB / " << resources.totalRam << " MB   ";
    displayProgressBar(ramPercentage);
    std::cout << "\n\n";

    // Disk usage
    int diskPercentage = (resources.usedDisk * 100) / resources.totalDisk;
    std::cout << "Disk Usage: " << resources.usedDisk << " MB / " << resources.totalDisk << " MB   ";
    displayProgressBar(diskPercentage);
    std::cout << "\n\n";

    // CPU usage
    std::cout << "CPU Usage:\n";
    for (int i = 0; i < resources.cpuCores; i++)
    {
        std::cout << "  Core " << (i + 1) << ": ";
        displayProgressBar(resources.cpuUsage[i]);
        std::cout << "\n";
    }
    std::cout << "\n";

    // Network
    std::cout << "Network: ↑ " << resources.networkUp << " KB/s  ↓ " << resources.networkDown << " KB/s\n\n";

    // System temperature
    std::cout << "Temperature: " << std::fixed << std::setprecision(1) << resources.temperature << "°C   ";
    if (resources.temperature < 50.0f)
    {
        std::cout << "Normal";
    }
    else if (resources.temperature < 70.0f)
    {
        std::cout << "Warm";
    }
    else
    {
        std::cout << "Hot!";
    }
    std::cout << "\n\n";

    // Battery
    std::cout << "Battery: ";
    displayProgressBar(resources.batteryLevel);
    std::cout << "\n\n";

    // Additional details if requested
    if (showDetailed)
    {
        std::cout << "======= Detailed System Information =======\n\n";

        std::cout << "Memory Allocation:\n";
        std::cout << "  - User Applications: " << (resources.usedRam * 65) / 100 << " MB\n";
        std::cout << "  - System Processes: " << (resources.usedRam * 25) / 100 << " MB\n";
        std::cout << "  - Cache: " << (resources.usedRam * 10) / 100 << " MB\n\n";

        std::cout << "Disk Breakdown:\n";
        std::cout << "  - System Files: " << (resources.usedDisk * 20) / 100 << " MB\n";
        std::cout << "  - User Data: " << (resources.usedDisk * 60) / 100 << " MB\n";
        std::cout << "  - Applications: " << (resources.usedDisk * 15) / 100 << " MB\n";
        std::cout << "  - Other: " << (resources.usedDisk * 5) / 100 << " MB\n\n";

        std::cout << "Uptime: 3 hours 27 minutes\n\n";
    }

    // Menu
    std::cout << "======= Menu =======\n";
    std::cout << "1. " << (paused ? "Resume" : "Pause") << " monitoring\n";
    std::cout << "2. Toggle detailed view\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter choice: ";
}

void displayMenu()
{
    std::cout << "\n===== System Monitor Menu =====\n";
    std::cout << "1. " << (paused ? "Resume" : "Pause") << " monitoring\n";
    std::cout << "2. Toggle detailed view\n";
    std::cout << "3. Exit\n";
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

    int pid = std::stoi(argv[1]);
    int memoryRequired = std::stoi(argv[2]);
    int diskRequired = std::stoi(argv[3]);

    // Print task information
    std::cout << "Starting System Monitor (PID: " << pid << ")" << std::endl;
    std::cout << "Memory required: " << memoryRequired << " MB" << std::endl;
    std::cout << "Disk required: " << diskRequired << " MB" << std::endl;

    // Register signal handler
    signal(SIGINT, signalHandler);

    // Initialize simulated system resources
    resources.totalRam = 8192;    // 8 GB
    resources.usedRam = 2048;     // 2 GB
    resources.totalDisk = 512000; // 500 GB
    resources.usedDisk = 256000;  // 250 GB
    resources.cpuCores = 8;
    resources.cpuUsage.resize(resources.cpuCores, 0);
    for (int i = 0; i < resources.cpuCores; i++)
    {
        std::uniform_int_distribution<int> cpuDist(10, 80);
        resources.cpuUsage[i] = cpuDist(gen);
    }
    resources.networkUp = 128;     // 128 KB/s
    resources.networkDown = 256;   // 256 KB/s
    resources.temperature = 45.5f; // 45.5°C
    resources.batteryLevel = 78;   // 78%

    // Create a separate thread for resource monitoring
    std::thread monitorThread([&]()
                              {
        while (running) {
            if (!paused) {
                updateResources();
                displayResources();
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } });

    // Main input loop
    int choice;
    while (running)
    {
        if (paused)
        {
            displayMenu();
            std::cin >> choice;

            switch (choice)
            {
            case 1: // Resume
                paused = false;
                std::cout << "Resuming monitoring..." << std::endl;
                break;

            case 2: // Toggle detailed view
                showDetailed = !showDetailed;
                std::cout << "Detailed view: " << (showDetailed ? "ON" : "OFF") << std::endl;
                break;

            case 3: // Exit
                running = false;
                break;

            default:
                std::cout << "Invalid choice" << std::endl;
                break;
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (std::cin.peek() != EOF)
            {
                std::cin >> choice;

                switch (choice)
                {
                case 1: // Pause
                    paused = true;
                    std::cout << "Monitoring paused. Press 1 to resume." << std::endl;
                    break;

                case 2: // Toggle detailed view
                    showDetailed = !showDetailed;
                    break;

                case 3: // Exit
                    running = false;
                    break;
                }
            }
        }
    }

    // Wait for monitoring thread to finish
    if (monitorThread.joinable())
    {
        monitorThread.join();
    }

    std::cout << "System Monitor exiting..." << std::endl;
    return 0;
}