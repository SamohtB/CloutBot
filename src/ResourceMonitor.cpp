#include "ResourceMonitor.h"

ResourceMonitor::ResourceMonitor(int maxResources) : availableResources(maxResources) {}

void ResourceMonitor::tryEnter()
{
    std::unique_lock<std::mutex> lock(this->guard);

    while (availableResources == 0)
    {
        this->condition.wait(lock);
    }

    if (availableResources > 0)
    {
        --availableResources;
    }
}

void ResourceMonitor::reportExit()
{
    std::lock_guard<std::mutex> lock(this->guard);
    availableResources++;
    this->condition.notify_one();
}