# Product Requirements Document (PRD)

## Title: Operating System Simulator

## Course: Operating Systems Lab – Spring 2025

## Language: C

## Interface: Terminal-Based (No GUI)

## Compatibility: macOS and Linux (POSIX compliant systems)

---

## 1. Objective

To simulate an operating system in a terminal environment using C++. The simulation must cover process management, resource allocation, multitasking, interrupts, and scheduling.

---

## 2. Initial Boot Requirements

- Display the name of the OS on startup using a terminal message.
- Simulate loading using a sleep command (e.g., `sleep(3)`).
- Ask the user to input:
  - Amount of RAM (e.g., 2GB)
  - Hard Drive space (e.g., 256GB)
  - Number of CPU cores (e.g., 8)
- Inputs should have default values which the user can accept by pressing enter.
- Inputs can also be given when starting the OS like ./os_simulator 2 256 4

These values will determine system constraints for the OS simulation.

---

## 3. Core Functionality and System Behavior

### 3.1 Resource Management

- RAM, Hard Drive, and CPU cores are limited based on user input.
- Every task must communicate its memory and disk requirements.
- If resources are not available, the task must be terminated.

### 3.2 Process Creation

- Each task must be a separate compiled program.
- Use EXEC commands to start each task in a new terminal.
- Function calls are not allowed for task execution.

### 3.3 Process Structure

- Each process must:
  - Be launched with its own terminal to avoid output conflicts.
  - Notify the main OS process of its resource needs.
  - Be accepted only if RAM and disk space are available.
  - Be rejected or terminated if resources are insufficient.

---

## 4. Task Requirements

- Minimum of 15 distinct tasks must be implemented.
- Examples include:
  - Notepad (with autosave to file)
  - Calculator
  - Clock
  - Create file
  - Copy file
  - Delete file
  - Move file
  - File information viewer
  - Mini-games (e.g., Minesweeper)
  - Other varied tasks such as timers or media playback simulations

Each task must behave as follows:

- Allocate required memory and disk.
- Be added to the ready queue upon creation.
- Be independently executable and terminable.
- Have a key to terminate the task and to minimize the tas

---

## 5. Multitasking

### 5.1 Multitasking Behavior

- The Operating System must support true multitasking.
- Multiple tasks should run concurrently based on resource availability.
- Tasks must be able to coexist in the system's READY queue and be scheduled to run based on available CPU cores.
- The OS must simulate multitasking by ensuring multiple tasks can:
  - Run simultaneously (e.g., game and music running together).
  - Be minimized and resumed later.
  - Switch between active and background tasks.
- Multitasking should follow the process lifecycle defined (NEW → READY → RUNNING → BLOCKED → TERMINATED).
- The OS must handle context switching between tasks properly using the designed scheduler and synchronization techniques.
- The scheduler must guarantee that if a task is blocked or minimized, other tasks continue to execute.

---

## 6. Scheduling

- The operating system must support multiple concurrent tasks.
- Implement multilevel queues for scheduling:
  - Each level uses a different technique (e.g., FCFS, priority).
- A ready queue will handle and dispatch tasks to available cores.
- Use of synchronization (mutual exclusion, semaphores, condition variables) is mandatory.

---

## 7. Interrupts and Task Management

- Each task must support manual interruption:
  - Minimize (task remains in memory but does not execute)
  - Close (task is terminated and resources freed)
- Interruptions must simulate task moving to a "Blocked" state.
- Blocked tasks can resume execution when appropriate.

---

## 8. Process Lifecycle

- Tasks must progress through standard lifecycle:
  - Created → Ready → Running → Blocked → Terminated
- Turnaround time must be set in code (not during execution).
- Upon completion, processes are removed from RAM.

---

## 9. File and Storage Behavior

- Data saved by tasks should be written to simulated hard disk (using file I/O).
- Tasks like Notepad must autosave files periodically.
- Disk space usage must be tracked and limited.

---

## 10. User and Kernel Modes

- The system must have two modes:
  - **User Mode**: limited control over processes.
  - **Kernel Mode**: full access to close or delete running programs and memory.
- Switching to kernel mode should enable privileged operations.

---

## 11. User Interface and Instructions

- In terminal mode, display a list of available tasks.
- Each task must support:
  - Close: fully terminate the task.
  - Minimize: hide the task, keep it in memory.
  - Switch: return to or resume a minimized task.
- Auto-start background services like clock and calendar after boot.
- Display resource usage status.

---

## 12. Task Types and Simulation

- **Interactive Tasks** (e.g., Calculator): require constant user input. Should have keys to close and minimize.
- **Background Tasks** (e.g., music playback): run silently.
- **Processing Tasks** (e.g., file copying): do computation without input.

All types must:

- Be added to the ready queue.
- Respect synchronization protocols.
- Only run if enough RAM and cores are available.

---

## 13. Shutdown Procedure

- The OS must allow user to shut down at any time.
- All tasks must be closed properly.
- Free all used memory and storage.
- Display a farewell message.

---

## 14. OS Concepts to Implement

The following concepts must be fully implemented:

- Multitasking
- Context switching
- Resource allocation
- User mode and kernel mode
- Process creation
- Threads
- EXEC command usage
- Synchronization using:
  - Mutual exclusion
  - Semaphore
  - Condition variables
- Multilevel queue scheduling

---

## 15. Platform Compatibility

- All features must work on both **macOS** and **Linux**.
- Use only standard C++ and POSIX-compliant libraries.
- Do not use low-level or complex third-party libraries.

---

## 16. Summary of Rules

- No direct function calls for task execution — use new processes only.
- Each task must be a standalone program.
- Tasks must not exceed allocated memory or disk limits.
- Manual interrupts (close/minimize) must be supported.
- Kernel mode must allow termination of any task.

---

## 17. Deliverables

- Source code for all components (OS and tasks)
- Makefile or compilation script
- README with:
  - Setup instructions
  - Task descriptions
  - How to use the OS simulator

---

## End of PRD
