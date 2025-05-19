#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

using namespace std;

void clearScreen()
{
    cout << "\033[2J\033[1;1H"; // ANSI escape sequence to clear screen
}

double calculate(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return b != 0 ? a / b : 0;
    default:
        return 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Usage: calculator <pid> <memory_required> <disk_required>" << endl;
        return 1;
    }

    int pid = stoi(argv[1]);
    int memoryRequired = stoi(argv[2]);
    int diskRequired = stoi(argv[3]);

    cout << "Calculator started with PID: " << pid << endl;
    cout << "Memory: " << memoryRequired << " MB, Disk: " << diskRequired << " MB" << endl;
    this_thread::sleep_for(chrono::seconds(2));

    double num1, num2, result;
    char op;
    bool running = true;

    while (running)
    {
        clearScreen();
        cout << "===== Calculator (PID: " << pid << ") =====" << endl;
        cout << "Enter calculation (e.g., 5 + 3) or 'q' to quit:" << endl;
        cout << "> ";

        string input;
        getline(cin, input);

        if (input == "q" || input == "Q")
        {
            running = false;
            continue;
        }

        // Parse input
        istringstream iss(input);
        if (iss >> num1 >> op >> num2)
        {
            result = calculate(num1, num2, op);
            cout << "Result: " << num1 << " " << op << " " << num2 << " = " << result << endl;
        }
        else
        {
            cout << "Invalid input format. Please try again." << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    cout << "Calculator task completed." << endl;
    return 0;
}
