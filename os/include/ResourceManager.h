#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager {
private:
    int totalMemory;
    int totalDisk;
    int totalCores;
    int availableMemory;
    int availableDisk;
    int availableCores;

public:
    ResourceManager(int totalMemory, int totalDisk, int totalCores);
    ~ResourceManager();

    bool allocateResources(int memory, int disk, int cores);
    void releaseResources(int memory, int disk, int cores);
    int getAvailableMemory() const;
    int getAvailableDisk() const;
    int getAvailableCores() const;
    void printResourceStatus() const;
};

#endif // RESOURCE_MANAGER_H 
