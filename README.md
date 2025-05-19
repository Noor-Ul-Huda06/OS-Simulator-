# OS-Simulator

A custom Operating System Simulator designed to mimic the functionalities of a real operating system. This project provides a hands-on experience with process management, resource allocation, CPU scheduling, and multitasking with an interactive interface.

## Project Structure

The project is organized into several modules, each handling different aspects of the operating system simulation:

- **build/**: Contains compiled object files.
- **include/**: Contains header files for the project.
  - `OSSystem.h`
  - `Process.h`
  - `ResourceManager.h`
  - `Scheduler.h`
- **src/**: Contains the source code for the project.
  - `OSSystem.cpp`
  - `Process.cpp`
  - `ResourceManager.cpp`
  - `Scheduler.cpp`
  - `main.cpp`
- **tasks/**: Contains a Makefile for building the project.
  - `Makefile`

## Project Details

### Process Management
- **Create Processes**: Users can create new processes and specify their resource requirements.
- **Monitor Processes**: View the status and resource usage of running processes.
- **Manage Processes**: Close, minimize, or restore processes as needed.

### Resource Allocation
- **RAM Management**: Allocate and monitor RAM usage for each process.
- **HDD Management**: Allocate and monitor HDD usage for each process.
- **CPU Management**: Allocate and monitor CPU core usage for each process.

### CPU Scheduling
- **First-Come-First-Serve (FCFS)**: Execute processes in the order they arrive.
- **Round Robin**: Execute processes in a cyclic order with a fixed time quantum.
- **Priority Scheduling**: Execute processes based on their priority levels.

### Multitasking
- **Background Tasks**: Run multiple tasks in the background and manage them effectively.
- **Task Switching**: Switch between different tasks and monitor their execution.

### Interactive Interface
- **User-Friendly Menu**: Navigate through different options with a colorful and intuitive menu system.
- **Real-Time Monitoring**: Monitor the status and resource usage of running processes in real-time.

### Additional Functionalities
- **Calculator**: Perform basic arithmetic operations.
- **Calendar**: View the current month's calendar.
- **Clock**: Display the current time.
- **File Manager**: Create, move, copy, delete, and view file information.
- **Minesweeper**: Play a simplified version of Minesweeper.
- **Music Player**: Play background music.
- **Notepad**: Create and save text files.
- **System Monitor**: Monitor system resource usage.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
