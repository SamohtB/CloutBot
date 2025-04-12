#include "ThreadPool.h"
#include "PoolWorkerThread.h"
#include "ResourceMonitor.h"
#include "Debug.h"

ThreadPool::ThreadPool(String name, int worker_count) : name(name), workerCount(worker_count)
{
	for (int i = 0; i < workerCount; i++)
	{
		
		this->inactiveThreadList.push(std::make_shared<PoolWorkerThread>(i, this));
	}

	this->workerCountMonitor = std::make_unique<ResourceMonitor>(workerCount);
	this->taskCountMonitor = std::make_unique<ResourceMonitor>(0);
}

ThreadPool::~ThreadPool()
{
	this->stopScheduler();
	this->activeThreadList.clear();

	while (!this->inactiveThreadList.empty())
	{
		this->inactiveThreadList.pop();
	}

	while (!this->pendingActions.empty())
	{
		this->pendingActions.pop();
	}

}

void ThreadPool::startScheduler()
{
	this->isRunning = true;
	this->start();
}

void ThreadPool::stopScheduler()	
{
	this->isRunning = false;
}

void ThreadPool::scheduleTask(std::shared_ptr<IWorkerAction> action)
{
	this->pendingActions.push(action);
	this->taskCountMonitor->reportExit(); //increase tasks by 1
}

void ThreadPool::run()
{
	while (this->isRunning)
	{
		this->taskCountMonitor->tryEnter();
		this->workerCountMonitor->tryEnter();

		WorkerPtr worker_thread = this->inactiveThreadList.front();
		this->inactiveThreadList.pop();
		this->activeThreadList[worker_thread->getThreadID()] = worker_thread;

		worker_thread->assignTask(this->pendingActions.front());
		worker_thread->start();
		this->pendingActions.pop();
	}
}

void ThreadPool::onFinishedTask(int threadID)
{
	std::lock_guard<std::mutex> lock(this->workerMutex);

	if (this->activeThreadList[threadID] != nullptr)
	{
		WorkerPtr worker = this->activeThreadList[threadID];
		this->activeThreadList.erase(threadID);
		this->inactiveThreadList.push(worker);
	}

	this->workerCountMonitor->reportExit();
}
