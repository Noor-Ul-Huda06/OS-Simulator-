#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <signal.h>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
#include <iomanip>

namespace fs = std::filesystem;
using namespace std;


bool running = true;
string currentDir = "simulated_disk";

// Signal handler for graceful shutdown
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "File Manager shutting down..." << std::endl;
        running = false;
    }
}

// Function to create a directory
bool createDirectory(const std::string &path)
{
    try
    {
        fs::create_directories(path);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return false;
    }
}

// Function to list files and directories
void listFiles(const std::string &path)
{
    try
    {
        if (!fs::exists(path))
        {
            std::cout << "Directory does not exist: " << path << std::endl;
            return;
        }

        std::cout << "\nContents of " << path << ":\n"
                  << std::endl;
        std::cout << std::left << std::setw(30) << "Name"
                  << std::setw(10) << "Type"
                  << std::setw(10) << "Size (B)" << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        for (const auto &entry : fs::directory_iterator(path))
        {
            std::string type = entry.is_directory() ? "DIR" : "FILE";
            uintmax_t fileSize = entry.is_regular_file() ? fs::file_size(entry) : 0;

            std::cout << std::left << std::setw(30) << entry.path().filename().string()
                      << std::setw(10) << type
                      << std::setw(10) << fileSize << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error listing files: " << e.what() << std::endl;
    }
}

// Function to create a file
bool createFile(const std::string &path, const std::string &content)
{
    try
    {
        std::ofstream file(path);
        if (!file)
        {
            return false;
        }
        file << content;
        file.close();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error creating file: " << e.what() << std::endl;
        return false;
    }
}

// Function to read a file
std::string readFile(const std::string &path)
{
    try
    {
        if (!fs::exists(path))
        {
            return "File does not exist: " + path;
        }

        std::ifstream file(path);
        if (!file)
        {
            return "Failed to open file: " + path;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return content;
    }
    catch (const std::exception &e)
    {
        return std::string("Error reading file: ") + e.what();
    }
}

// Function to delete a file or directory
bool deleteFileOrDir(const std::string &path)
{
    try
    {
        if (!fs::exists(path))
        {
            std::cout << "File or directory does not exist: " << path << std::endl;
            return false;
        }

        uintmax_t count = fs::remove_all(path);
        std::cout << "Removed " << count << " files/directories" << std::endl;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error deleting file or directory: " << e.what() << std::endl;
        return false;
    }
}

// Function to copy a file
bool copyFile(const std::string &source, const std::string &destination)
{
    try
    {
        if (!fs::exists(source))
        {
            std::cout << "Source file does not exist: " << source << std::endl;
            return false;
        }

        fs::copy(source, destination, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return false;
    }
}

// Function to move/rename a file
bool moveFile(const std::string &source, const std::string &destination)
{
    try
    {
        if (!fs::exists(source))
        {
            std::cout << "Source file does not exist: " << source << std::endl;
            return false;
        }

        fs::rename(source, destination);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error moving file: " << e.what() << std::endl;
        return false;
    }
}

void displayMenu()
{
    std::cout << "\n===== File Manager Menu =====\n";
    std::cout << "Current Directory: " << currentDir << std::endl;
    std::cout << "1. List Files\n";
    std::cout << "2. Create File\n";
    std::cout << "3. Create Directory\n";
    std::cout << "4. Read File\n";
    std::cout << "5. Delete File/Directory\n";
    std::cout << "6. Copy File\n";
    std::cout << "7. Move/Rename File\n";
    std::cout << "8. Change Directory\n";
    std::cout << "9. Exit\n";
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
    std::cout << "Starting File Manager (PID: " << pid << ")" << std::endl;
    std::cout << "Memory required: " << memoryRequired << " MB" << std::endl;
    std::cout << "Disk required: " << diskRequired << " MB" << std::endl;

    // Register signal handler
    signal(SIGINT, signalHandler);

    // Create the simulated disk directory if it doesn't exist
    createDirectory(currentDir);

    int choice;
    while (running)
    {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

        switch (choice)
        {
        case 1: // List Files
            listFiles(currentDir);
            break;

        case 2:
        { // Create File
            std::string filename, content;
            std::cout << "Enter filename: ";
            std::getline(std::cin, filename);
            std::cout << "Enter file content (type ':q' on a new line to finish):" << std::endl;

            content = "";
            std::string line;
            while (std::getline(std::cin, line) && line != ":q")
            {
                content += line + "\n";
            }

            std::string fullPath = currentDir + "/" + filename;
            if (createFile(fullPath, content))
            {
                std::cout << "File created successfully: " << fullPath << std::endl;
            }
            else
            {
                std::cout << "Failed to create file" << std::endl;
            }
            break;
        }

        case 3:
        { // Create Directory
            std::string dirname;
            std::cout << "Enter directory name: ";
            std::getline(std::cin, dirname);

            std::string fullPath = currentDir + "/" + dirname;
            if (createDirectory(fullPath))
            {
                std::cout << "Directory created successfully: " << fullPath << std::endl;
            }
            else
            {
                std::cout << "Failed to create directory" << std::endl;
            }
            break;
        }

        case 4:
        { // Read File
            std::string filename;
            std::cout << "Enter filename: ";
            std::getline(std::cin, filename);

            std::string fullPath = currentDir + "/" + filename;
            std::string content = readFile(fullPath);

            std::cout << "\n----- File Content -----\n"
                      << std::endl;
            std::cout << content << std::endl;
            std::cout << "\n----- End of File -----\n"
                      << std::endl;
            break;
        }

        case 5:
        { // Delete File/Directory
            std::string path;
            std::cout << "Enter file/directory name to delete: ";
            std::getline(std::cin, path);

            std::string fullPath = currentDir + "/" + path;
            if (deleteFileOrDir(fullPath))
            {
                std::cout << "Deleted successfully: " << fullPath << std::endl;
            }
            else
            {
                std::cout << "Failed to delete" << std::endl;
            }
            break;
        }

        case 6:
        { // Copy File
            std::string source, destination;
            std::cout << "Enter source filename: ";
            std::getline(std::cin, source);
            std::cout << "Enter destination filename: ";
            std::getline(std::cin, destination);

            std::string fullSource = currentDir + "/" + source;
            std::string fullDestination = currentDir + "/" + destination;

            if (copyFile(fullSource, fullDestination))
            {
                std::cout << "File copied successfully" << std::endl;
            }
            else
            {
                std::cout << "Failed to copy file" << std::endl;
            }
            break;
        }

        case 7:
        { // Move/Rename File
            std::string source, destination;
            std::cout << "Enter source filename: ";
            std::getline(std::cin, source);
            std::cout << "Enter destination filename: ";
            std::getline(std::cin, destination);

            std::string fullSource = currentDir + "/" + source;
            std::string fullDestination = currentDir + "/" + destination;

            if (moveFile(fullSource, fullDestination))
            {
                std::cout << "File moved/renamed successfully" << std::endl;
            }
            else
            {
                std::cout << "Failed to move/rename file" << std::endl;
            }
            break;
        }

        case 8:
        { // Change Directory
            std::string newDir;
            std::cout << "Enter directory name (.. for parent, / for root): ";
            std::getline(std::cin, newDir);

            if (newDir == "/")
            {
                currentDir = "simulated_disk";
            }
            else if (newDir == "..")
            {
                size_t lastSlash = currentDir.find_last_of('/');
                if (lastSlash != std::string::npos && currentDir != "simulated_disk")
                {
                    currentDir = currentDir.substr(0, lastSlash);
                }
                if (currentDir.empty() || currentDir.find("simulated_disk") == std::string::npos)
                {
                    currentDir = "simulated_disk";
                }
            }
            else
            {
                std::string fullPath = currentDir + "/" + newDir;
                if (fs::exists(fullPath) && fs::is_directory(fullPath))
                {
                    currentDir = fullPath;
                }
                else
                {
                    std::cout << "Directory does not exist: " << fullPath << std::endl;
                }
            }
            break;
        }

        case 9: // Exit
            running = false;
            break;

        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    std::cout << "File Manager exiting..." << std::endl;
    return 0;
}
