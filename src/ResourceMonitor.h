#pragma once

#include <condition_variable>

class ResourceMonitor
{
public:
	ResourceMonitor(int maxResources);

	void tryEnter();
	void reportExit();

private:
	std::mutex guard;
	std::condition_variable condition;
	int availableResources = 0;
};

