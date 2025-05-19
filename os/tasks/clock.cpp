#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <signal.h>
#include <string>

using namespace std;

bool running = true;
bool displayMode = true; // true = digital, false = analog

// Signal handler for graceful shutdown
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        cout << "Clock shutting down..." << endl;
        running = false;
    }
}

// Function to display digital clock
void displayDigitalClock()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm *localTime = localtime(&currentTime);

    cout << "\r" << setfill('0')
         << setw(2) << localTime->tm_hour << ":"
         << setw(2) << localTime->tm_min << ":"
         << setw(2) << localTime->tm_sec
         << " " << (1900 + localTime->tm_year) << "-"
         << setw(2) << (localTime->tm_mon + 1) << "-"
         << setw(2) << localTime->tm_mday
         << flush;
}

// Function to display ASCII analog clock
void displayAnalogClock()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm *localTime = localtime(&currentTime);

    int hour = localTime->tm_hour % 12;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    // Clear screen for better display
    cout << "\033[2J\033[1;1H";

    cout << "      .         " << endl;
    cout << "    .   .       " << endl;
    cout << "  .       .     " << endl;
    cout << " .    " << setw(2) << setfill('0') << hour << "    .   " << endl;
    cout << ".             . " << endl;
    cout << ".     @       . " << endl;
    cout << ".     |       . " << endl;
    cout << " .    |     .   " << endl;
    cout << "  .   |   .     " << endl;
    cout << "    . | .       " << endl;
    cout << "      '         " << endl;
    cout << "Time: " << setfill('0')
         << setw(2) << hour << ":"
         << setw(2) << minute << ":"
         << setw(2) << second << endl;
}

void displayMenu()
{
    cout << "\n\n===== Clock Menu =====\n";
    cout << "1. Toggle display mode (Digital/Analog)\n";
    cout << "2. Exit\n";
    cout << "Enter choice: ";
}

int main(int argc, char *argv[])
{
    // Check command line arguments (pid, memory, disk)
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <pid> <memory_required> <disk_required>" << endl;
        return 1;
    }

    int pid = stoi(argv[1]);
    int memoryRequired = stoi(argv[2]);
    int diskRequired = stoi(argv[3]);

    // Print task information
    cout << "Starting Clock (PID: " << pid << ")" << endl;
    cout << "Memory required: " << memoryRequired << " MB" << endl;
    cout << "Disk required: " << diskRequired << " MB" << endl;

    // Register signal handler
    signal(SIGINT, signalHandler);

    cout << "Clock started. Press 'q' followed by Enter to show menu." << endl;

    // Create a separate thread for handling user input
    thread inputThread([&]()
                      {
        char input;
        while (running) {
            cin >> input;
            if (input == 'q' || input == 'Q') {
                int choice;
                displayMenu();
                cin >> choice;
                
                switch (choice) {
                    case 1:
                        displayMode = !displayMode;
                        cout << "Switched to " << (displayMode ? "Digital" : "Analog") << " display mode." << endl;
                        break;
                    case 2:
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice" << endl;
                        break;
                }
            }
        } });

    // Main clock display loop
    while (running)
    {
        if (displayMode)
        {
            displayDigitalClock();
        }
        else
        {
            displayAnalogClock();
        }

        // Update every second
        this_thread::sleep_for(chrono::seconds(1));
    }

    if (inputThread.joinable())
    {
        inputThread.join();
    }

    cout << "\nClock exiting..." << endl;
    return 0;
}
