#include "FlagMonitor.h"

FlagMonitor::FlagMonitor(bool initialCondition)
{
	this->isOpen = initialCondition;
}

void FlagMonitor::tryEnter()
{
	std::unique_lock<std::mutex> lock(this->guard);

	while (!isOpen)
	{
		this->condition.wait(lock);
	}

	this->isOpen = false;
}

void FlagMonitor::reportExit()
{
	std::lock_guard<std::mutex> lock(this->guard);
	this->isOpen = true;
	this->condition.notify_one();
}
