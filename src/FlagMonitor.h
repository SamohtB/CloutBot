#pragma once
#include <condition_variable>

class FlagMonitor
{
public:
	FlagMonitor(bool initialCondition = false);

	void tryEnter();
	void notifyOne();

private:
	std::mutex guard;
	std::condition_variable condition;
	bool isOpen = false;
}