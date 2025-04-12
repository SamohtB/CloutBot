#pragma once
#include <queue>
#include <unordered_map>
#include <mutex>
#include <memory>
#include "IETThread.h"
#include "IFinishedTask.h"

class PoolWorkerThread;
class IWorkerAction;
class IFinishedTask;
class ResourceMonitor;

class ThreadPool : public IETThread, public IFinishedTask
{
private:
	using String = std::string;
	using WorkerPtr = std::shared_ptr<PoolWorkerThread>;
	using ThreadList = std::queue<WorkerPtr>;
	using ActiveThreadMap = std::unordered_map<int, WorkerPtr>;
	using ActionList = std::queue<std::shared_ptr<IWorkerAction>>;

public:
	ThreadPool(String name, int worker_count);
	~ThreadPool();

	void startScheduler();
	void stopScheduler();
	void scheduleTask(std::shared_ptr<IWorkerAction> action);

private:
	void run() override;
	void onFinishedTask(int threadID) override;

	String name;
	bool isRunning = false;
	int workerCount = 0;

	ActiveThreadMap activeThreadList;
	ThreadList inactiveThreadList;
	ActionList pendingActions;

	std::unique_ptr<ResourceMonitor> workerCountMonitor = nullptr;
	std::unique_ptr<ResourceMonitor> taskCountMonitor = nullptr;
	std::mutex workerMutex;
	
};

