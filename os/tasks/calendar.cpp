#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Usage: calendar <pid> <memory_required> <disk_required>" << endl;
        return 1;
    }

    int pid = stoi(argv[1]);
    int memoryRequired = stoi(argv[2]);
    int diskRequired = stoi(argv[3]);

    cout << "Calendar started with PID: " << pid << endl;
    cout << "Memory: " << memoryRequired << " MB, Disk: " << diskRequired << " MB" << endl;

    // Main loop to display current date/time
    for (int i = 0; i < 60; ++i)
    {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);

        cout << "\033[2J\033[1;1H"; // Clear screen
        cout << "===== Calendar =====" << endl;
        cout << "Current date and time: " << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S") << endl;
        cout << "Running for " << i << " seconds" << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "Calendar task completed." << endl;
    return 0;
}
