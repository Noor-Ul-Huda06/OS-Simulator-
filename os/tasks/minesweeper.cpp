#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
//#include <iomanip>
#include <signal.h>

using namespace std;

bool running = true;

// Game board dimensions
const int ROWS = 8;
const int COLS = 8;
const int MINES = 10;

// Cell states
enum CellState
{
    COVERED,
    UNCOVERED,
    FLAGGED
};

// Cell structure
struct Cell
{
    bool hasMine;
    int adjacentMines;
    CellState state;
};

// Game board
vector<vector<Cell>> board(ROWS, vector<Cell>(COLS));

// Signal handler for graceful shutdown
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        cout << "Minesweeper shutting down..." << endl;
        running = false;
    }
}

// Initialize the game board
void initializeBoard()
{
    // Initialize all cells
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j].hasMine = false;
            board[i][j].adjacentMines = 0;
            board[i][j].state = COVERED;
        }
    }

    // Place mines randomly
    int minesPlaced = 0;
    while (minesPlaced < MINES)
    {
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (!board[row][col].hasMine)
        {
            board[row][col].hasMine = true;
            minesPlaced++;

            // Update adjacent cells
            for (int i = max(0, row - 1); i <= min(ROWS - 1, row + 1); i++)
            {
                for (int j = max(0, col - 1); j <= min(COLS - 1, col + 1); j++)
                {
                    if (i == row && j == col)
                        continue;
                    board[i][j].adjacentMines++;
                }
            }
        }
    }
}

// Print the game board
void printBoard(bool showAll = false)
{
    cout << "    ";
    for (int j = 0; j < COLS; j++)
    {
        cout << j << " ";
    }
    cout << endl;

    cout << "   ";
    for (int j = 0; j < COLS; j++)
    {
        cout << "--";
    }
    cout << endl;

    for (int i = 0; i < ROWS; i++)
    {
        cout << i << " | ";
        for (int j = 0; j < COLS; j++)
        {
            if (showAll || board[i][j].state == UNCOVERED)
            {
                if (board[i][j].hasMine)
                {
                    cout << "* ";
                }
                else
                {
                    if (board[i][j].adjacentMines == 0)
                    {
                        cout << ". ";
                    }
                    else
                    {
                        cout << board[i][j].adjacentMines << " ";
                    }
                }
            }
            else if (board[i][j].state == FLAGGED)
            {
                cout << "F ";
            }
            else
            {
                cout << "# ";
            }
        }
        cout << endl;
    }
}

// Uncover a cell recursively
bool uncoverCell(int row, int col)
{
    // Check bounds
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    {
        return true;
    }

    // Skip if already uncovered or flagged
    if (board[row][col].state == UNCOVERED || board[row][col].state == FLAGGED)
    {
        return true;
    }

    // Uncover the cell
    board[row][col].state = UNCOVERED;

    // Check if it's a mine
    if (board[row][col].hasMine)
    {
        return false;
    }

    // If it's a zero, uncover adjacent cells
    if (board[row][col].adjacentMines == 0)
    {
        for (int i = max(0, row - 1); i <= min(ROWS - 1, row + 1); i++)
        {
            for (int j = max(0, col - 1); j <= min(COLS - 1, col + 1); j++)
            {
                if (i == row && j == col)
                    continue;
                uncoverCell(i, j);
            }
        }
    }

    return true;
}

// Toggle flag on a cell
void toggleFlag(int row, int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    {
        return;
    }

    if (board[row][col].state == COVERED)
    {
        board[row][col].state = FLAGGED;
    }
    else if (board[row][col].state == FLAGGED)
    {
        board[row][col].state = COVERED;
    }
}

// Check if the game is won
bool checkWin()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (!board[i][j].hasMine && board[i][j].state != UNCOVERED)
            {
                return false;
            }
        }
    }
    return true;
}

void displayMenu()
{
    cout << "\n===== Minesweeper Menu =====\n";
    cout << "1. Uncover a cell\n";
    cout << "2. Toggle flag\n";
    cout << "3. New game\n";
    cout << "4. Exit\n";
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
    cout << "Starting Minesweeper (PID: " << pid << ")" << endl;
    cout << "Memory required: " << memoryRequired << " MB" << endl;
    cout << "Disk required: " << diskRequired << " MB" << endl;

    // Register signal handler
    signal(SIGINT, signalHandler);

    // Seed random number generator
    srand(time(nullptr));

    // Initialize game
    initializeBoard();

    bool gameOver = false;
    bool gameWon = false;

    cout << "\nWelcome to Minesweeper!" << endl;
    cout << "Find all cells without mines to win." << endl;
    cout << "There are " << MINES << " mines on the board." << endl;

    while (running && !gameOver)
    {
        // Print board
        cout << endl;
        printBoard();

        // Display menu
        displayMenu();

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        { // Uncover a cell
            int row, col;
            cout << "Enter row and column (e.g., 2 3): ";
            cin >> row >> col;

            if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
            {
                bool safe = uncoverCell(row, col);
                if (!safe)
                {
                    cout << "\nBOOM! You hit a mine!" << endl;
                    gameOver = true;
                }
                else
                {
                    gameWon = checkWin();
                    if (gameWon)
                    {
                        cout << "\nCongratulations! You found all the safe cells!" << endl;
                        gameOver = true;
                    }
                }
            }
            else
            {
                cout << "Invalid coordinates. Try again." << endl;
            }
            break;
        }

        case 2:
        { // Toggle flag
            int row, col;
            cout << "Enter row and column to flag/unflag: ";
            cin >> row >> col;

            if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
            {
                toggleFlag(row, col);
            }
            else
            {
                cout << "Invalid coordinates. Try again." << endl;
            }
            break;
        }

        case 3: // New game
            initializeBoard();
            gameOver = false;
            gameWon = false;
            cout << "\nNew game started!" << endl;
            break;

        case 4: // Exit
            running = false;
            break;

        default:
            cout << "Invalid choice. Try again." << endl;
            break;
        }
    }

    // Show the final board with all mines
    if (gameOver)
    {
        cout << "\nFinal board:" << endl;
        printBoard(true);
    }

    cout << "Minesweeper exiting..." << endl;
    return 0;
}
