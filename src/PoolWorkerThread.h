#pragma once
#include "IETThread.h"

class IWorkerAction;
class IFinishedTask;

class PoolWorkerThread : public IETThread
{
public:
	PoolWorkerThread(int id, IFinishedTask* finished_task);
	~PoolWorkerThread();

	int getThreadID();
	void assignTask(std::shared_ptr<IWorkerAction> action);

private:
	void run() override;

	int id = 0;
	std::shared_ptr<IWorkerAction> action = nullptr;
	IFinishedTask* finishedTask = nullptr;
};

