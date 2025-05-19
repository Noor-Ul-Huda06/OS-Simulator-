#include "../include/ResourceManager.h"
#include <iostream>
//#include <algorithm>

using std::cout;
using std::endl;

ResourceManager::ResourceManager(int totalMemory, int totalDisk, int totalCores)
    : totalMemory(totalMemory), totalDisk(totalDisk), totalCores(totalCores),
      availableMemory(totalMemory), availableDisk(totalDisk), availableCores(totalCores)
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::allocateResources(int memory, int disk, int cores)
{
    if (memory <= availableMemory && disk <= availableDisk && cores <= availableCores)
    {
        availableMemory -= memory;
        availableDisk -= disk;
        availableCores -= cores;
        return true;
    }
    return false;
}

void ResourceManager::releaseResources(int memory, int disk, int cores)
{
    availableMemory += memory;
    availableDisk += disk;
    availableCores += cores;
}

int ResourceManager::getAvailableMemory() const
{
    return availableMemory;
}

int ResourceManager::getAvailableDisk() const
{
    return availableDisk;
}

int ResourceManager::getAvailableCores() const
{
    return availableCores;
}

void ResourceManager::printResourceStatus() const
{
    cout << "\n=== Resource Status ===" << endl;
    cout << "Memory: " << availableMemory << "/" << totalMemory << " MB" << endl;
    cout << "Disk: " << availableDisk << "/" << totalDisk << " MB" << endl;
    cout << "CPU Cores: " << availableCores << "/" << totalCores << endl;
    cout << "=====================" << endl;
} 
