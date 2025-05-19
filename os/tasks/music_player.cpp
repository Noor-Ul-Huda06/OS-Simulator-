#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <signal.h>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <iomanip>

using namespace std;

atomic<bool> running{true};
atomic<bool> playing{false};
atomic<bool> shuffleMode{false};
mutex playlistMutex;

struct Song
{
    string title;
    string artist;
    int duration; // in seconds
    string genre;
};

vector<Song> playlist;
int currentSongIndex = 0;
int playbackPosition = 0;

// Signal handler for graceful shutdown
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        cout << "Music Player shutting down..." << endl;
        running = false;
        playing = false;
    }
}

// Function to simulate playing a song
void playSong()
{
    while (running)
    {
        if (playing && !playlist.empty())
        {
            lock_guard<mutex> lock(playlistMutex);
            if (currentSongIndex >= 0 && currentSongIndex < playlist.size())
            {
                Song currentSong = playlist[currentSongIndex];

                // Print playback progress
                cout << "\rNow playing: " << currentSong.title << " - " << currentSong.artist
                     << " [";

                // Progress bar
                int barLength = 20;
                int position = (playbackPosition * barLength) / currentSong.duration;
                for (int i = 0; i < barLength; i++)
                {
                    if (i < position)
                        cout << "=";
                    else if (i == position)
                        cout << ">";
                    else
                        cout << " ";
                }

                cout << "] " << playbackPosition << "/" << currentSong.duration << "s" << flush;

                // Increment playback position
                playbackPosition++;

                // If song is finished, move to next
                if (playbackPosition >= currentSong.duration)
                {
                    playbackPosition = 0;
                    if (shuffleMode)
                    {
                        // Random next song
                        currentSongIndex = rand() % playlist.size();
                    }
                    else
                    {
                        // Sequential next song
                        currentSongIndex = (currentSongIndex + 1) % playlist.size();
                    }
                }
            }
        }

        // Wait a second for the next update
        this_thread::sleep_for(chrono::seconds(1));
    }
}

// Initialize playlist with some sample songs
void initializePlaylist()
{
    lock_guard<mutex> lock(playlistMutex);
    playlist = {
        {"Bohemian Rhapsody", "Queen", 355, "Rock"},
        {"Imagine", "John Lennon", 183, "Pop"},
        {"Billie Jean", "Michael Jackson", 292, "Pop"},
        {"Stairway to Heaven", "Led Zeppelin", 482, "Rock"},
        {"Thriller", "Michael Jackson", 357, "Pop"},
        {"Like a Rolling Stone", "Bob Dylan", 369, "Rock"},
        {"Smells Like Teen Spirit", "Nirvana", 301, "Rock"},
        {"Yesterday", "The Beatles", 125, "Pop"},
        {"Hotel California", "Eagles", 391, "Rock"},
        {"Sweet Child O' Mine", "Guns N' Roses", 355, "Rock"}
    };
}

// Add a song to the playlist
void addSong()
{
    lock_guard<mutex> lock(playlistMutex);
    Song newSong;

    cout << "Enter song title: ";
    cin.ignore();
    getline(cin, newSong.title);

    cout << "Enter artist: ";
    getline(cin, newSong.artist);

    cout << "Enter duration (seconds): ";
    cin >> newSong.duration;

    cout << "Enter genre: ";
    cin.ignore();
    getline(cin, newSong.genre);

    playlist.push_back(newSong);
    cout << "Song added to playlist!" << endl;
}

// Display playlist
void displayPlaylist()
{
    lock_guard<mutex> lock(playlistMutex);
    cout << "\n===== Playlist =====\n" << endl;

    if (playlist.empty())
    {
        cout << "Playlist is empty." << endl;
        return;
    }

    for (size_t i = 0; i < playlist.size(); i++)
    {
        cout << (i == currentSongIndex ? "▶ " : "  ")
             << (i + 1) << ". " << playlist[i].title << " - " << playlist[i].artist
             << " (" << playlist[i].duration / 60 << ":" << setw(2) << setfill('0') << playlist[i].duration % 60
             << ") [" << playlist[i].genre << "]" << endl;
    }
}

// Save playlist to file
void savePlaylist()
{
    lock_guard<mutex> lock(playlistMutex);
    string filename = "simulated_disk/playlist.txt";

    // Create directory if it doesn't exist
    system("mkdir -p simulated_disk");

    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }

    for (const auto& song : playlist)
    {
        outFile << song.title << "\n"
                << song.artist << "\n"
                << song.duration << "\n"
                << song.genre << "\n";
    }

    outFile.close();
    cout << "Playlist saved to " << filename << endl;
}

// Load playlist from file
void loadPlaylist()
{
    lock_guard<mutex> lock(playlistMutex);
    string filename = "simulated_disk/playlist.txt";

    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Error: Could not open file for reading." << endl;
        return;
    }

    playlist.clear();
    Song song;

    while (getline(inFile, song.title))
    {
        getline(inFile, song.artist);
        inFile >> song.duration;
        inFile.ignore(); // Skip newline
        getline(inFile, song.genre);

        playlist.push_back(song);
    }

    inFile.close();
    cout << "Playlist loaded from " << filename << endl;

    if (!playlist.empty())
    {
        currentSongIndex = 0;
        playbackPosition = 0;
    }
}

void displayMenu()
{
    cout << "\n\n===== Music Player Menu =====\n";
    cout << "1. Play/Pause\n";
    cout << "2. Next Song\n";
    cout << "3. Previous Song\n";
    cout << "4. Display Playlist\n";
    cout << "5. Add Song\n";
    cout << "6. Toggle Shuffle\n";
    cout << "7. Save Playlist\n";
    cout << "8. Load Playlist\n";
    cout << "9. Exit\n";
    cout << "Enter choice: ";
}

int main(int argc, char* argv[])
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
    cout << "Starting Music Player (PID: " << pid << ")" << endl;
    cout << "Memory required: " << memoryRequired << " MB" << endl;
    cout << "Disk required: " << diskRequired << " MB" << endl;

    // Register signal handler
    signal(SIGINT, signalHandler);

    // Seed random for shuffle
    srand(time(nullptr));

    // Initialize the playlist
    initializePlaylist();

    // Start the playback thread
    thread playbackThread(playSong);
    playbackThread.detach();

    int choice;
    while (running)
    {
        displayMenu();
        cin >> choice;

        switch (choice)
        {
            case 1: // Play/Pause
                playing = !playing;
                cout << (playing ? "Playing" : "Paused") << endl;
                break;

            case 2: // Next Song
            {
                lock_guard<mutex> lock(playlistMutex);
                playbackPosition = 0;
                if (shuffleMode)
                    currentSongIndex = rand() % playlist.size();
                else
                    currentSongIndex = (currentSongIndex + 1) % playlist.size();
                cout << "Now playing: " << playlist[currentSongIndex].title
                     << " by " << playlist[currentSongIndex].artist << endl;
            }
                break;

            case 3: // Previous Song
            {
                lock_guard<mutex> lock(playlistMutex);
                playbackPosition = 0;
                if (shuffleMode)
                    currentSongIndex = rand() % playlist.size();
                else
                    currentSongIndex = (currentSongIndex - 1 + playlist.size()) % playlist.size();
                cout << "Now playing: " << playlist[currentSongIndex].title
                     << " by " << playlist[currentSongIndex].artist << endl;
            }
                break;

            case 4: // Display Playlist
                displayPlaylist();
                break;

            case 5: // Add Song
                addSong();
                break;

            case 6: // Toggle Shuffle
                shuffleMode = !shuffleMode;
                cout << "Shuffle mode: " << (shuffleMode ? "ON" : "OFF") << endl;
                break;

            case 7: // Save Playlist
                savePlaylist();
                break;

            case 8: // Load Playlist
                loadPlaylist();
                break;

            case 9: // Exit
                running = false;
                playing = false;
                break;

            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }

    cout << "Music Player exiting..." << endl;
    return 0;
}
